#include "Sandbox2D.h"
#include "Boozy/Renderer/Renderer.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::OnAttach()
{
    m_SquareVertexArray = Boozy::VertexArray::Create();

    // 逆时针
    float squareVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    Boozy::Ref<Boozy::VertexBuffer> squareVertexBuffer;
    squareVertexBuffer.reset(Boozy::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    squareVertexBuffer->SetLayout({
        {Boozy::ShaderDataType::Float3, "a_Position"}
        });
    m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

    Boozy::Ref<Boozy::IndexBuffer> squareIndexBuffer;
    uint32_t sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
    squareIndexBuffer.reset(Boozy::IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(uint32_t)));
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

    m_FlatColorShader = Boozy::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Boozy::Timestep delta)
{
    m_CameraController.OnUpdate(delta);

    Boozy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer::BeginScene(m_CameraController.GetCamera());

    m_FlatColorShader->Bind();
    m_FlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);

    Boozy::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Boozy::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Boozy::Event & e)
{

}
