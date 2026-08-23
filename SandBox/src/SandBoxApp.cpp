#include <Boozy.h>
#include <imgui.h>

BZ_INIT_LOGGER("SandBox"); // 初始化本文件 SandBox 日志器

class ExampleLayer : public Boozy::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
        m_CameraPosition(0.0f), m_CameraRotation(0.0f)
    {
        m_VertexArray.reset(Boozy::VertexArray::Create());

        // 逆时针
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<Boozy::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Boozy::VertexBuffer::Create(vertices, sizeof(vertices)));

        Boozy::BufferLayout layout = {
            {Boozy::ShaderDataType::Float3, "a_Position"},
            {Boozy::ShaderDataType::Float4, "a_Color"}
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        std::shared_ptr<Boozy::IndexBuffer> indexBuffer;
        uint32_t indices[3] = { 0, 1, 2 };
        indexBuffer.reset(Boozy::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(
            #version 460 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec4 v_Color;

            void main()
            {
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            })";

        std::string fragmentSrc = R"(
            #version 460 core

            in vec4 v_Color;

            layout(location = 0) out vec4 color;

            void main()
            {
                color = v_Color;
            })";

        m_Shader.reset(Boozy::Shader::Create(vertexSrc, fragmentSrc));
    }

    void OnUpdate() override
    {
        if (Boozy::Input::IsKeyPressed(BZ_KEY_W))
            m_CameraPosition.y += m_CameraMoveSpeed;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_A))
            m_CameraPosition.x -= m_CameraMoveSpeed;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_S))
            m_CameraPosition.y -= m_CameraMoveSpeed;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_D))
            m_CameraPosition.x += m_CameraMoveSpeed;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_E))
            m_CameraRotation -= m_CameraRotateSpeed;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_Q))
            m_CameraRotation += m_CameraRotateSpeed;

        Boozy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Boozy::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Boozy::Renderer::BeginScene(m_Camera);
        Boozy::Renderer::Submit(m_Shader, m_VertexArray);
        Boozy::Renderer::EndScene();

    }

    void OnImGuiRender() override
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void OnEvent(Boozy::Event& event) override
    {

    }

private:
    std::shared_ptr<Boozy::Shader> m_Shader;
    std::shared_ptr<Boozy::VertexArray> m_VertexArray;

    Boozy::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 0.01f;
    float m_CameraRotation;
    float m_CameraRotateSpeed = 1.0f;
};

class SandBox : public Boozy::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }

    ~SandBox()
    {

    }

};

Boozy::Application* Boozy::CreateApplication()
{
    return new SandBox();
}