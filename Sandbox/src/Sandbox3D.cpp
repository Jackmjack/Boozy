#include "Sandbox3D.h"
#include <imgui.h>

Sandbox3D::Sandbox3D()
    : Layer("Sandbox3D"), m_Camera(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
{
    m_Camera.SetPosition({ 0.0f, 0.0f, 5.0f });
    m_Camera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void Sandbox3D::OnAttach()
{

    m_CubeTransform = Boozy::Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

    Boozy::Vertex vertices[24] = {
        // +Z 面，法线 (0, 0, 1)
        { { -0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f } },

        // -Z 面，法线 (0, 0, -1)
        { {  0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },

        // +X 面，法线 (1, 0, 0)
        { { 0.5f, -0.5f,  0.5f},   {1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f, -0.5f},   {1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f } },
        { { 0.5f,  0.5f, -0.5f},   {1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f } },
        { { 0.5f,  0.5f,  0.5f},   {1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f } },

        // -X 面，法线 (-1, 0, 0)
        { { -0.5f, -0.5f, -0.5f},  { -1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f } },
        { { -0.5f, -0.5f,  0.5f},  { -1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f},  { -1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f, -0.5f},  { -1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f } },

        // +Y 面，法线 (0, 1, 0)
        { {  0.5f,  0.5f, -0.5f},   { 0.0f,  1.0f,  0.0f}, { 0.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f},   { 0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f},   { 0.0f,  1.0f,  0.0f}, { 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f},   { 0.0f,  1.0f,  0.0f}, { 0.0f, 1.0f } },

        // -Y 面，法线 (0, -1, 0)
        { { -0.5f, -0.5f, -0.5f},   { 0.0f, -1.0f,  0.0f}, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f, -0.5f},   { 0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f},   { 0.0f, -1.0f,  0.0f}, { 1.0f, 1.0f } },
        { { -0.5f, -0.5f,  0.5f},   { 0.0f, -1.0f,  0.0f}, { 0.0f, 1.0f } }
    };

    uint32_t indices[36] = {
        // +Z
        0, 1, 2,   0, 2, 3,
        // -Z
        4, 5, 6,   4, 6, 7,
        // +X
        8, 9, 10,  8, 10, 11,
        // -X
        12, 13, 14, 12, 14, 15,
        // +Y
        16, 17, 18, 16, 18, 19,
        // -Y
        20, 21, 22, 20, 22, 23,
    };

    Boozy::BufferLayout layout = {
        { Boozy::ShaderDataType::Float3, "a_Position" },
        { Boozy::ShaderDataType::Float3, "a_Normal" },
        { Boozy::ShaderDataType::Float2, "a_UV" }
    };

    m_CubeMesh.reset(new Boozy::Mesh(vertices, 24, indices, 36, layout));

    m_FlatColorShader3D = Boozy::Shader::Create("assets/shaders/FlatColor3D.glsl");
}
void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Boozy::Timestep delta)
{
    m_CubeTransform.SetRotation(m_CubeTransform.GetRotation() + m_CubeRotationSpeed * (float)delta);

    Boozy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer3D::BeginScene(m_Camera);

    Boozy::Renderer3D::DrawMesh(m_CubeTransform, m_CubeMesh, m_FlatColorShader3D);

    Boozy::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{

}

void Sandbox3D::OnEvent(Boozy::Event& event)
{

}
