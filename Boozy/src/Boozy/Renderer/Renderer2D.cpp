#include "bzpch.h"
#include "Renderer2D.h"
#include "Boozy/Core/Core.h"
#include "Boozy/Core/Log.h"
#include "Boozy/Renderer/VertexArray.h"
#include "Boozy/Renderer/Shader.h"
#include "Boozy/Renderer/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {

    struct Renderer2DData
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
    };

    static Renderer2DData* s_Data;

    void Renderer2D::Init()
    {
        BZ_ASSERT(!s_Data, "Renderer2D already initialized!");
        s_Data = new Renderer2DData();
        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        Ref<VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"}
            });
        s_Data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

        Ref<IndexBuffer> squareIndexBuffer;
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

        s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
        s_Data = nullptr;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        Renderer2D::DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->FlatColorShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

}