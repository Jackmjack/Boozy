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
        DirectionalLight Light;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer3DData* s_Data;

    void Renderer3D::Init()
    {
        BZ_ASSERT(!s_Data, "Renderer3D already initialized!");
        s_Data = new Renderer3DData();

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t white = 0xffffffff;
        s_Data->WhiteTexture->SetData(&white, sizeof(white));
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
    }

    void Renderer3D::BeginScene(const Camera& camera, const DirectionalLight& light)
    {
        Renderer3D::BeginScene(camera);
        s_Data->Light = light;
    }

    void Renderer3D::EndScene()
    {

    }

    void Renderer3D::DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Shader>& shader)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        shader->SetMat4("u_Model", transform.GetTransformMatrix());

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
    }

    void Renderer3D::DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Material>& material)
    {
        material->Bind();

        const Ref<Shader>& shader = material->GetShader();
        shader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        shader->SetMat4("u_Model", transform.GetTransformMatrix());

        shader->SetFloat3("u_LightDirection", s_Data->Light.Direction);
        shader->SetFloat3("u_LightColor", s_Data->Light.Color * s_Data->Light.Intensity);
        shader->SetFloat3("u_AmbientColor", s_Data->Light.Ambient);

        const Ref<Texture2D>& texture = material->GetTexture() ? material->GetTexture() : s_Data->WhiteTexture;
        texture->Bind(0);
        shader->SetInt("u_Texture", 0);

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
    }
}