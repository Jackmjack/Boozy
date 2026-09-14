#include "bzpch.h"
#include "Renderer3D.h"
#include "Boozy/Renderer/RenderCommand.h"

#include "Boozy/Core/Log.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy
{
    struct Renderer3DData
    {
        glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        std::vector<Light> Lights;
        glm::vec3 Ambient = glm::vec3(0.1f);
        Ref<Texture2D> WhiteTexture;

        Ref<VertexArray> FloorVertexArray;
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
    }

    void Renderer3D::Init()
    {
        BZ_ASSERT(!s_Data, "Renderer3D already initialized!");
        s_Data = new Renderer3DData();

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t white = 0xffffffff;
        s_Data->WhiteTexture->SetData(&white, sizeof(white));

        s_Data->FloorVertexArray = VertexArray::Create();
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
        shader->SetFloat("u_FadeStart", 50.0f);
        shader->SetFloat("u_FadeEnd", 100.0f);

        UploadLightsUniforms(shader);

        s_Data->FloorVertexArray->Bind();
        RenderCommand::DrawArrays(3);
    }
}