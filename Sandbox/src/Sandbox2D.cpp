#include "Sandbox2D.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::OnAttach()
{
    m_Texture = Boozy::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Boozy::Timestep delta)
{
    m_CameraController.OnUpdate(delta);

    Boozy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer2D::BeginScene(m_CameraController.GetCamera());


    Boozy::Renderer2D::DrawQuad({ -0.5f, 0.0f }, { 0.9f, 1.0f }, m_SquareColor);
    Boozy::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.9f, 2.0f }, m_SquareColor);
    Boozy::Renderer2D::DrawQuad({ 1.5f, 1.0f, -0.9f }, { 0.9f, 3.0f }, m_SquareColor);

    Boozy::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.8f }, { 10.0f, 10.0f }, m_Texture);


    Boozy::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Boozy::Event & event)
{
    m_CameraController.OnEvent(event);
}
