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
    };

    static Renderer3DData* s_Data;

    void Renderer3D::Init()
    {
        BZ_ASSERT(!s_Data, "Renderer3D already initialized!");
        s_Data = new Renderer3DData();
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
}