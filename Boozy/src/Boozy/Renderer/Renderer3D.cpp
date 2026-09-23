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
        glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        std::vector<Light> Lights;
        glm::vec3 Ambient = glm::vec3(0.1f);
        Ref<Texture2D> WhiteTexture;

        Ref<VertexArray> FloorVertexArray;

        // ------------------ Shadow ------------------
        Ref<Shader> ShadowShader;
        std::vector<ShadowSlot> Slots;
        std::vector<Ref<FrameBuffer>> FBOs;
        std::vector<Ref<FrameBuffer>> CubeFBOs;
        float ShadowSoftness = 1.0f;

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
            fbo = FrameBuffer::Create({ 1024, 1024 });

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
                // ---- 光源空间矩阵 ----
                // 盒子必须同时罩住 ①投影体 ②它在地板上的落点：IsInShadow() 对盒外片元直接 return 0.0
                // （当成被照亮），所以接收面一旦在盒外，那块阴影根本不会出现，而不是"精度变差"。
                // 茶壶平移到 y=5、本地包围盒中心 y≈1.6 → 世界中心 y≈6.6；落点 = (x+0.5y, 0, z+0.3y)
                // （光方向归一化后水平分量正好是 0.5 / 0.3），自转后包围球半径约 4，落点最远再偏 6。
                // 用固定球而不是每帧 AABB：矩阵每帧完全一致 → 纹素栅格不动 → 阴影不抖。
                const glm::vec3 center(0.0f, 4.0f, -12.0f);
                const float radius = 12.0f;

                const glm::vec3 lightDir = glm::normalize(s_Data->Lights[i].Direction);   // 传播方向
                const glm::vec3 up = std::abs(lightDir.y) > 0.99f ? glm::vec3(0.0f, 0.0f, 1.0f)
                    : glm::vec3(0.0f, 1.0f, 0.0f);

                // 光源在 -lightDir 那一侧。写成 + 会让光与阴影整个颠倒。
                const glm::mat4 lightView = glm::lookAt(center - lightDir * (radius * 2.0f), center, up);
                const glm::mat4 lightProj = glm::ortho(-radius, radius, -radius, radius, 0.1f, radius * 4.0f);

                ShadowSlot slot = { i, lightProj * lightView, s_Data->FBOs[i]};
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

        RenderCommand::EnablePolygonOffset();
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
        RenderCommand::DisablePolygonOffset();

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
        shader->SetFloat("u_FadeStart", 50.0f);
        shader->SetFloat("u_FadeEnd", 100.0f);
        shader->SetFloat3("u_FadeColor", glm::vec3(0.0f));

        UploadLightsUniforms(shader);

        s_Data->FloorVertexArray->Bind();
        RenderCommand::DrawArrays(3);
    }
}