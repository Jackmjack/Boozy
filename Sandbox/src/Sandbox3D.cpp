#include "Sandbox3D.h"
#include <imgui.h>

Sandbox3D::Sandbox3D()
    : Layer("Sandbox3D"), m_CameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
{
}

void Sandbox3D::OnAttach()
{
    Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Disabled);

    m_Light.Color = glm::vec3(0.7f, 0.8f, 1.0f);

    Boozy::Ref<Boozy::Shader> shader = Boozy::Shader::Create("assets/shaders/Lit3D.glsl");
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_Material.reset(new Boozy::Material(shader, color));
    m_Material->SetDoubleSided(true);

    m_Model.reset(new Boozy::Model("assets/models/teapot.obj"));
    m_ModelTransform = Boozy::Transform({ 0.0f, 0.0f, -12.0f }, m_Rotation, { 1.0f, 1.0f, 1.0f });
}

void Sandbox3D::OnDetach()
{
    Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Normal);
}

void Sandbox3D::OnUpdate(Boozy::Timestep delta)
{

    if (Boozy::Input::IsKeyPressed(BZ_KEY_ESCAPE))
        Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Normal);

    m_CameraController.OnUpdate(delta);

    m_Rotation = { m_Rotation.x + m_RotationSpeed * delta, m_Rotation.y, m_Rotation.z + m_RotationSpeed * 2 * delta };
    m_ModelTransform = Boozy::Transform({ 0.0f, 0.0f, -12.0f }, m_Rotation, { 1.0f, 1.0f, 1.0f });

    Boozy::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer3D::BeginScene(m_CameraController.GetCamera(), m_Light);

    if (m_Model && m_Model->IsValid())
        for (const auto& entry : m_Model->GetMeshes())
            Boozy::Renderer3D::DrawMesh(m_ModelTransform, entry.Mesh, m_Material);

    Boozy::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{

}

void Sandbox3D::OnEvent(Boozy::Event& event)
{
    m_CameraController.OnEvent(event);
}
