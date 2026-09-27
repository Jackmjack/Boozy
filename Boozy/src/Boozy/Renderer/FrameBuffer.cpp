#include "bzpch.h"
#include "FrameBuffer.h"

#include "Boozy/Core/Log.h"
#include "Boozy/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {

    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps& props)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLFrameBuffer>(props);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}