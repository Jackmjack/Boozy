#include "Sandbox3D.h"
#include <imgui.h>

Sandbox3D::Sandbox3D()
    : Layer("Sandbox3D"), m_CameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
{
}

void Sandbox3D::OnAttach()
{
    Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Disabled);

    m_Texture = Boozy::Texture2D::Create("assets/textures/Checkerboard.png");

    m_Model.reset(new Boozy::Model("assets/models/teapot.obj"));
    m_ModelTransform = Boozy::Transform({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

    m_FlatColorShader3D = Boozy::Shader::Create("assets/shaders/FlatColor3D.glsl");

    m_TextureShader3D = Boozy::Shader::Create("assets/shaders/Texture3D.glsl");
    m_TextureShader3D->Bind();
    m_TextureShader3D->SetInt("u_Texture", 0);
    m_TextureShader3D->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
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

    Boozy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer3D::BeginScene(m_CameraController.GetCamera());

    m_Texture->Bind();

    if (m_Model && m_Model->IsValid())
        for (const auto& entry : m_Model->GetMeshes())
            Boozy::Renderer3D::DrawMesh(m_ModelTransform, entry.Mesh, m_FlatColorShader3D);

    Boozy::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{

}

void Sandbox3D::OnEvent(Boozy::Event& event)
{
    m_CameraController.OnEvent(event);
}
