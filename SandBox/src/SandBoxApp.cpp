#include <Boozy.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

        Boozy::Ref<Boozy::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Boozy::VertexBuffer::Create(vertices, sizeof(vertices)));

        Boozy::BufferLayout layout = {
            {Boozy::ShaderDataType::Float3, "a_Position"},
            {Boozy::ShaderDataType::Float4, "a_Color"}
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        Boozy::Ref<Boozy::IndexBuffer> indexBuffer;
        uint32_t indices[3] = { 0, 1, 2 };
        indexBuffer.reset(Boozy::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(
            #version 460 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec4 v_Color;

            void main()
            {
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        // ===========================================================

        m_SquareVertexArray.reset(Boozy::VertexArray::Create());

        // 逆时针
        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };

        Boozy::Ref<Boozy::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(Boozy::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        Boozy::BufferLayout squareLayout = {
            {Boozy::ShaderDataType::Float3, "a_Position"},
            {Boozy::ShaderDataType::Float2, "a_Texcoord"}
        };

        squareVertexBuffer->SetLayout(squareLayout);
        m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

        Boozy::Ref<Boozy::IndexBuffer> squareIndexBuffer;
        uint32_t sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
        squareIndexBuffer.reset(Boozy::IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(uint32_t)));
        m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

        std::string squareVertexSrc = R"(
            #version 460 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            void main()
            {
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            })";

        std::string squareFragmentSrc = R"(
            #version 460 core

            layout(location = 0) out vec4 color;

            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0);
            })";

        m_SquareShader.reset(Boozy::Shader::Create(squareVertexSrc, squareFragmentSrc));

        std::string textureVertexSrc = R"(
            #version 460 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;

            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            })";

        std::string textureFragmentSrc = R"(
            #version 460 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform sampler2D u_Texture;

            void main()
            {
                color = texture(u_Texture, v_TexCoord);
            })";

        m_TextureShader.reset(Boozy::Shader::Create(textureVertexSrc, textureFragmentSrc));

        m_Texture = Boozy::Texture2D::Create("assets/textures/Checkerboard.png");
        m_AlphaTest = Boozy::Texture2D::Create("assets/textures/AlphaTest.png");

        m_TextureShader->Bind();
        m_TextureShader->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Boozy::Timestep delta) override
    {
        if (Boozy::Input::IsKeyPressed(BZ_KEY_W))
            m_CameraPosition.y += m_CameraMoveSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_A))
            m_CameraPosition.x -= m_CameraMoveSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_S))
            m_CameraPosition.y -= m_CameraMoveSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_D))
            m_CameraPosition.x += m_CameraMoveSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_E))
            m_CameraRotation -= m_CameraRotateSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_Q))
            m_CameraRotation += m_CameraRotateSpeed * delta;

        Boozy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Boozy::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Boozy::Renderer::BeginScene(m_Camera);

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        m_SquareShader->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int x = 0; x < 20; x++)
        {
            for (int y = 0; y < 20; y++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Boozy::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
            }
        }

        m_Texture->Bind();
        Boozy::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        m_AlphaTest->Bind();
        Boozy::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        //Boozy::Renderer::Submit(m_Shader, m_VertexArray);

        Boozy::Renderer::EndScene();

    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnEvent(Boozy::Event& event) override
    {

    }

private:
    Boozy::Ref<Boozy::Shader> m_Shader;
    Boozy::Ref<Boozy::VertexArray> m_VertexArray;

    Boozy::Ref<Boozy::Shader> m_SquareShader, m_TextureShader;
    Boozy::Ref<Boozy::VertexArray> m_SquareVertexArray;
    glm::vec3 m_SquareColor{ 0.2f, 0.3f, 0.8f };

    Boozy::Ref<Boozy::Texture2D> m_Texture;
    Boozy::Ref<Boozy::Texture2D> m_AlphaTest;

    Boozy::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 1.0f;
    float m_CameraRotation;
    float m_CameraRotateSpeed = 180.0f;
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