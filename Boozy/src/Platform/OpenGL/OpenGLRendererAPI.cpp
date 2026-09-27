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

    const int32_t* OpenGLRendererAPI::GetViewport() const
    {
        static int32_t viewport[4] = { 0, 0, 0, 0 };
        glGetIntegerv(GL_VIEWPORT, viewport);
        return viewport;
    }

    uint32_t OpenGLRendererAPI::GetFrameBuffer() const
    {
        GLint fbo = 0;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fbo);
        return (uint32_t)fbo;
    }

    void OpenGLRendererAPI::BindFrameBuffer(uint32_t rendererID)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::ClearDepth()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
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

    void OpenGLRendererAPI::EnablePolygonOffset()
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(4.0f, 40.0f);
    }

    void OpenGLRendererAPI::DisablePolygonOffset()
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    void OpenGLRendererAPI::BindTexture(uint32_t slot, uint32_t rendererID) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, rendererID);
    }

    void OpenGLRendererAPI::BindTextureCube(uint32_t slot, uint32_t rendererID) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        BZ_ASSERT(vertexArray->GetIndexBuffer(), "VertexArray has no index buffer!");

        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawArrays(uint32_t vertexCount)
    {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexCount);
    }
}
