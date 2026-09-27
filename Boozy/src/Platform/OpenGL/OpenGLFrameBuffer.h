#pragma once

#include "Boozy/Renderer/FrameBuffer.h"

namespace Boozy {
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FrameBufferProps& props);
        virtual ~OpenGLFrameBuffer() override;

        virtual void Bind() const override;
        virtual void BindFace(uint32_t) const override;
        virtual void Unbind() const override;

        virtual const uint32_t GetDepthAttachment() const override { return m_ShadowMap; };
        virtual const uint32_t GetRendererID() const override { return m_RendererID; };
        virtual const FrameBufferProps& GetProps() const override { return m_Props; }
    private:
        uint32_t m_RendererID;
        uint32_t m_ShadowMap;

        FrameBufferProps m_Props;
    };
}

