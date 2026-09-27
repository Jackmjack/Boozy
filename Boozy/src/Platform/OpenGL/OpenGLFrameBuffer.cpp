#include "bzpch.h"
#include "OpenGLFrameBuffer.h"

#include "Boozy/Core/Log.h"

#include <glad/glad.h>

BZ_INIT_LOGGER("OpenGL");

namespace Boozy {

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProps& props)
        : m_Props(props)
    {
        if (props.IsCubeMap == false)
        {
            glGenFramebuffers(1, &m_RendererID);
            glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

            glGenTextures(1, &m_ShadowMap);
            glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Props.Width, m_Props.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTextureParameteri(m_ShadowMap, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTextureParameteri(m_ShadowMap, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(m_ShadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTextureParameteri(m_ShadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);
        }
        else
        {
            glGenFramebuffers(1, &m_RendererID);
            glGenTextures(1, &m_ShadowMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
            for (int f = 0; f < 6; f++)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f,
                    0, GL_DEPTH_COMPONENT, m_Props.Width, m_Props.Height, 0,
                    GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

            glTextureParameteri(m_ShadowMap, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTextureParameteri(m_ShadowMap, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_ShadowMap, 0);
        }

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            BZ_ERROR("Framebuffer is incomplete: 0x{:X}", (int)status);
        BZ_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        if (m_RendererID) glDeleteFramebuffers(1, &m_RendererID);
        if (m_ShadowMap) glDeleteTextures(1, &m_ShadowMap);
    }

    void OpenGLFrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Props.Width, m_Props.Height);
    }

    void OpenGLFrameBuffer::BindFace(uint32_t face) const
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_ShadowMap, 0);
        glViewport(0, 0, m_Props.Width, m_Props.Height);
    }

    void OpenGLFrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}