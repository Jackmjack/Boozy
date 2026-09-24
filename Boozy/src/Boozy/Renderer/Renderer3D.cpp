#include "bzpch.h"
#include "Renderer3D.h"
#include "Boozy/Renderer/RenderCommand.h"
#include "Boozy/Renderer/FrameBuffer.h"
#include "Boozy/Core/Log.h"

#include <glm/gtc/matrix_transform.hpp>

BZ_INIT_LOGGER("Renderer");

namespace Boozy
{
    struct ShadowSlot {
        int LightIndex = -1;
        glm::mat4 LightSpaceMatrix{ 1.0f };
        Ref<FrameBuffer> Map;
        std::vector<glm::mat4> CubeMatrixs{6};
    };

    struct Renderer3DData
    {
        glm::mat4 ViewMatrix = glm::mat4(1.0f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        std::vector<Light> Lights;
        glm::vec3 Ambient = glm::vec3(0.1f);
        Ref<Texture2D> WhiteTexture;

        Ref<VertexArray> FloorVertexArray;
        Ref<TextureCube> Skybox;

        // ------------------ Shadow ------------------
        Ref<Shader> ShadowShader;
        std::vector<ShadowSlot> Slots;
        std::vector<Ref<FrameBuffer>> FBOs;
        std::vector<Ref<FrameBuffer>> CubeFBOs;
        float ShadowSoftness = 1.0f;
        float ShadowRadius = 0.0f;
        uint32_t ShadowMapSize = 4096;
        float ShadowBiasScale = 2.5f;

        uint32_t MainFrameBuffer = 0;
        int32_t MainViewport[4] = { 0, 0, 0, 0 };
    };

    static Renderer3DData* s_Data;

    static void UploadLightsUniforms(Ref<Shader> shader)
    {
        shader->Bind();
        shader->SetFloat3("u_AmbientColor", s_Data->Ambient);

        int count = (int)std::min<size_t>(s_Data->Lights.size(), BZ_MAX_LIGHTS);
        int types[BZ_MAX_LIGHTS] = {};
        glm::vec3 positions[BZ_MAX_LIGHTS] = {};
        glm::vec3 directions[BZ_MAX_LIGHTS] = {};
        glm::vec3 colors[BZ_MAX_LIGHTS] = {};
        float intensities[BZ_MAX_LIGHTS] = {};
        float constants[BZ_MAX_LIGHTS] = {};
        float linears[BZ_MAX_LIGHTS] = {};
        float quadratics[BZ_MAX_LIGHTS] = {};
        float cosInners[BZ_MAX_LIGHTS] = {};
        float cosOuters[BZ_MAX_LIGHTS] = {};

        for (int i = 0; i < count; i++)
        {
            types[i] = (int)s_Data->Lights[i].Type;
            positions[i] = s_Data->Lights[i].Position;
            directions[i] = s_Data->Lights[i].Direction;
            colors[i] = s_Data->Lights[i].Color;
            intensities[i] = s_Data->Lights[i].Intensity;
            constants[i] = s_Data->Lights[i].Constant;
            linears[i] = s_Data->Lights[i].Linear;
            quadratics[i] = s_Data->Lights[i].Quadratic;
            cosInners[i] = glm::cos(glm::radians(s_Data->Lights[i].InnerCutOff));
            cosOuters[i] = glm::cos(glm::radians(s_Data->Lights[i].OuterCutOff));
        }

        shader->SetInt("u_LightCount", count);
        shader->SetIntArray("u_LightType", types, count);
        shader->SetFloat3Array("u_LightColor", colors, count);
        shader->SetFloatArray("u_LightIntensity", intensities, count);

        shader->SetFloat3Array("u_LightPosition", positions, count);

        shader->SetFloat3Array("u_LightDirection", directions, count);

        shader->SetFloatArray("u_LightConstant", constants, count);
        shader->SetFloatArray("u_LightLinear", linears, count);
        shader->SetFloatArray("u_LightQuadratic", quadratics, count);

        shader->SetFloatArray("u_LightCosInner", cosInners, count);
        shader->SetFloatArray("u_LightCosOuter", cosOuters, count);

        int shadowUnits[BZ_MAX_LIGHTS];
        int hasShadow[BZ_MAX_LIGHTS] = {};
        glm::mat4 shadowMatrices[BZ_MAX_LIGHTS];
        for (int i = 0; i < BZ_MAX_LIGHTS; i++) { shadowUnits[i] = i + 1; shadowMatrices[i] = glm::mat4(0.0f); }

        int cubeUnits[BZ_MAX_LIGHTS];
        int hasCube[BZ_MAX_LIGHTS] = {};
        glm::mat4 cubeMatrices[BZ_MAX_LIGHTS * 6];
        for (int i = 0; i < BZ_MAX_LIGHTS; i++) cubeUnits[i] = i + 1;
        for (int i = 0; i < BZ_MAX_LIGHTS * 6; i++) cubeMatrices[i] = glm::mat4(0.0f);

        for (const auto& slot : s_Data->Slots)
        {
            const int li = slot.LightIndex;

            if (s_Data->Lights[li].Type == LightType::Point)
            {
                cubeUnits[li] = li + 1;
                hasCube[li] = 1;
                for (int f = 0; f < 6; f++)
                    cubeMatrices[li * 6 + f] = slot.CubeMatrixs[f];
                RenderCommand::BindTextureCube(li + 1, slot.Map->GetDepthAttachment());
            }
            else {
                shadowUnits[li] = li + 1;
                hasShadow[li] = 1;
                shadowMatrices[li] = slot.LightSpaceMatrix;
                RenderCommand::BindTexture(li + 1, slot.Map->GetDepthAttachment());
            }
        }
        shader->SetIntArray("u_ShadowMap", shadowUnits, BZ_MAX_LIGHTS);
        shader->SetIntArray("u_HasShadow", hasShadow, BZ_MAX_LIGHTS);
        shader->SetMat4Array("u_LightSpaceMatrix", shadowMatrices, BZ_MAX_LIGHTS);

        shader->SetIntArray("u_ShadowCube", cubeUnits, BZ_MAX_LIGHTS);
        shader->SetIntArray("u_HasShadowCube", hasCube, BZ_MAX_LIGHTS);
        shader->SetMat4Array("u_MatrixCube", cubeMatrices, BZ_MAX_LIGHTS * 6);

        shader->SetFloat("u_ShadowSoftness", s_Data->ShadowSoftness);
        const float texelWorld = 2.0f * s_Data->ShadowRadius / (float)s_Data->ShadowMapSize;
        shader->SetFloat("u_ShadowNormalBias", texelWorld * s_Data->ShadowBiasScale);
    }

    void Renderer3D::Init()
    {
        BZ_ASSERT(!s_Data, "Renderer3D already initialized!");
        s_Data = new Renderer3DData();

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t white = 0xffffffff;
        s_Data->WhiteTexture->SetData(&white, sizeof(white));

        s_Data->FloorVertexArray = VertexArray::Create();

        s_Data->ShadowShader = Shader::Create("assets/shaders/ShadowMapping.glsl");

        s_Data->FBOs.resize(BZ_MAX_LIGHTS);
        for (auto& fbo : s_Data->FBOs)
            fbo = FrameBuffer::Create({ s_Data->ShadowMapSize, s_Data->ShadowMapSize });

        s_Data->CubeFBOs.resize(BZ_MAX_LIGHTS);
    }

    void Renderer3D::Shutdown()
    {
        delete s_Data;
        s_Data = nullptr;
    }

    void Renderer3D::BeginScene(const Camera& camera)
    {
        RenderCommand::EnableDepthTest();
        RenderCommand::EnableFaceCulling();
        RenderCommand::EnableBlending();

        s_Data->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        s_Data->ViewMatrix = camera.GetViewMatrix();
        s_Data->ProjectionMatrix = camera.GetProjectionMatrix();
        s_Data->Lights.clear();
        s_Data->Ambient = glm::vec3(0.0f);
    }

    void Renderer3D::BeginScene(const Camera& camera, const std::vector<Light>& lights, const glm::vec3 ambient)
    {
        Renderer3D::BeginScene(camera);

        if (lights.size() > BZ_MAX_LIGHTS)
            BZ_WARN("Light count {} exceeds BZ_MAX_LIGHTS {}, {} light(s) ignored",
                lights.size(), BZ_MAX_LIGHTS, lights.size() - BZ_MAX_LIGHTS);

        s_Data->Lights = lights;
        s_Data->Ambient = ambient;
    }

    void Renderer3D::EndScene()
    {

    }

    void Renderer3D::BeginShadow()
    {
        s_Data->Slots.clear();

        s_Data->MainFrameBuffer = RenderCommand::GetFrameBuffer();
        const int32_t* vp = RenderCommand::GetViewport();
        for (int i = 0; i < 4; i++)
            s_Data->MainViewport[i] = vp[i];

        for (int i = 0; i < (int)s_Data->Lights.size(); i++)
        {
            if (s_Data->Lights[i].Type == LightType::Directional)
            {
                ShadowSlot slot = { i, Light::MakeDirectionalShadowMatrix(s_Data->Lights[i], s_Data->ViewProjectionMatrix, s_Data->FBOs[i]->GetProps().Width, s_Data->ShadowRadius), s_Data->FBOs[i]};
                s_Data->Slots.push_back(slot);
            }
            else if (s_Data->Lights[i].Type == LightType::Point)
            {
                if (!s_Data->CubeFBOs[i])
                    s_Data->CubeFBOs[i] = FrameBuffer::Create({ 1024, 1024, true });

                ShadowSlot slot = { i, glm::mat4(1.0f), s_Data->CubeFBOs[i], Light::MakePointShadowMatrix(s_Data->Lights[i]) };
                s_Data->Slots.push_back(slot);
            }
            else if (s_Data->Lights[i].Type == LightType::Spot)
            {
                ShadowSlot slot = { i, Light::MakeSpotShadowMatrix(s_Data->Lights[i]), s_Data->FBOs[i]};
                s_Data->Slots.push_back(slot);
            }
        }

        RenderCommand::DisableFaceCulling();

        for (auto& slot : s_Data->Slots)
        {
            if (s_Data->Lights[slot.LightIndex].Type == LightType::Point)
            {
                slot.Map->Bind();
                for (uint32_t f = 0; f < 6; f++)
                {
                    slot.Map->BindFace(f);
                    RenderCommand::ClearDepth();
                }
            }
            else
            {
                slot.Map->Bind();
                RenderCommand::ClearDepth();
            }
        }
    }

    void Renderer3D::DrawShadow(const Transform& transform, const Ref<Mesh>& mesh)
    {
        for (auto& slot : s_Data->Slots)
        {
            if (s_Data->Lights[slot.LightIndex].Type == LightType::Point)
            {
                slot.Map->Bind();
                for (uint32_t f = 0; f < 6; f++) {
                    slot.Map->BindFace(f);
                    s_Data->ShadowShader->Bind();
                    s_Data->ShadowShader->SetMat4("u_LightSpaceMatrix", slot.CubeMatrixs[f]);
                    s_Data->ShadowShader->SetMat4("u_Model", transform.GetTransformMatrix());
                    mesh->Bind();
                    RenderCommand::DrawIndexed(mesh->GetVertexArray());
                }
                continue;
            }

            slot.Map->Bind();
            s_Data->ShadowShader->Bind();
            s_Data->ShadowShader->SetMat4("u_LightSpaceMatrix", slot.LightSpaceMatrix);
            s_Data->ShadowShader->SetMat4("u_Model", transform.GetTransformMatrix());
            mesh->Bind();
            RenderCommand::DrawIndexed(mesh->GetVertexArray());
        }
    }

    void Renderer3D::EndShadow()
    {
        for (auto& slot : s_Data->Slots)
            slot.Map->Unbind();

        RenderCommand::EnableFaceCulling();

        RenderCommand::BindFrameBuffer(s_Data->MainFrameBuffer);
        RenderCommand::SetViewport(s_Data->MainViewport[0], s_Data->MainViewport[1],
            (uint32_t)s_Data->MainViewport[2], (uint32_t)s_Data->MainViewport[3]);
    }

    void Renderer3D::DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Shader>& shader)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        shader->SetMat4("u_Model", transform.GetTransformMatrix());
        shader->SetInt("u_LightCount", 0);

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
    }

    void Renderer3D::DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Material>& material)
    {
        material->Bind();

        const Ref<Shader>& shader = material->GetShader();
        shader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        shader->SetMat4("u_Model", transform.GetTransformMatrix());

        UploadLightsUniforms(shader);

        const Ref<Texture2D>& texture = material->GetTexture() ? material->GetTexture() : s_Data->WhiteTexture;
        texture->Bind(0);
        shader->SetInt("u_Texture", 0);

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
    }

    void Renderer3D::DrawFloor(const Ref<Shader>& shader)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        shader->SetMat4("u_InvViewProjection", glm::inverse(s_Data->ViewProjectionMatrix));
        shader->SetFloat4("u_Color", glm::vec4(1.0f));
        shader->SetBool("u_GridEnabled", true);
        shader->SetFloat3("u_GridColor", glm::vec3(0.15f));
        shader->SetFloat("u_GridScale", 1.0f);
        shader->SetFloat("u_FadeStart", 90.0f);
        shader->SetFloat("u_FadeEnd", 100.0f);
        shader->SetFloat3("u_FadeColor", glm::vec3(0.0f));
        
        UploadLightsUniforms(shader);

        if (s_Data->Skybox)
        {
            s_Data->Skybox->Bind(BZ_MAX_LIGHTS + 1);
            shader->SetInt("u_Skybox", BZ_MAX_LIGHTS + 1);
            shader->SetBool("u_HasSkybox", true);
        }
        else
        {
            shader->SetBool("u_HasSkybox", false);
        }

        s_Data->FloorVertexArray->Bind();
        RenderCommand::DrawArrays(3);
    }

    void Renderer3D::DrawSkybox(const Ref<TextureCube>& skybox, const Ref<Shader>& shader)
    {
        s_Data->Skybox = skybox;

        RenderCommand::DisableDepthTest();

        glm::mat4 view = glm::mat4(glm::mat3(s_Data->ViewMatrix));

        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_Data->ProjectionMatrix * view);
        s_Data->Skybox->Bind(BZ_MAX_LIGHTS + 1);
        shader->SetInt("u_Skybox", BZ_MAX_LIGHTS + 1);
        RenderCommand::DrawArrays(36);

        RenderCommand::EnableDepthTest();
    }
}