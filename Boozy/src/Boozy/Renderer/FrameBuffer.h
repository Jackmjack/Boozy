#pragma once

#include "Boozy/Core/Core.h"

#include <cstdint>

namespace Boozy {

    struct FrameBufferProps
    {
        uint32_t Width = 0, Height = 0;
        bool IsCubeMap = false;
    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() = default;

        virtual void Bind() const = 0;
        virtual void BindFace(uint32_t face) const = 0;
        virtual void Unbind() const = 0;

        virtual const uint32_t GetDepthAttachment() const = 0;
        virtual const uint32_t GetRendererID() const = 0;
        virtual const FrameBufferProps& GetProps() const = 0;

        static Ref<FrameBuffer> Create(const FrameBufferProps& props);
    };

}

