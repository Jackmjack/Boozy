#include "bzpch.h"
#include "VertexArray.h"
#include "Boozy/Log.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {

    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLVertexArray();
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}