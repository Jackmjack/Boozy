#include "bzpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Boozy/Core/Log.h"

#include <glad/glad.h>

BZ_INIT_LOGGER("OpenGL");

namespace Boozy {
    void OpenGLRendererAPI::Init()
    {

    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::EnableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void OpenGLRendererAPI::DisableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::EnableFaceCulling()
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void OpenGLRendererAPI::DisableFaceCulling()
    {
        glDisable(GL_CULL_FACE);
    }

    void OpenGLRendererAPI::EnableBlending()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGLRendererAPI::DisableBlending()
    {
        glDisable(GL_BLEND);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        BZ_ASSERT(vertexArray->GetIndexBuffer(), "VertexArray has no index buffer!");
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawArrays(uint32_t vertexCount)
    {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexCount);
    }
}
