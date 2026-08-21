#include "bzpch.h"
#include "Buffer.h"
#include "Boozy/Log.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {

    uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:     return 4;
        case ShaderDataType::Float2:    return 4 * 2;
        case ShaderDataType::Float3:    return 4 * 3;
        case ShaderDataType::Float4:    return 4 * 4;
        case ShaderDataType::Mat3:      return 4 * 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4 * 4;
        case ShaderDataType::Int:       return 4;
        case ShaderDataType::Int2:      return 4 * 2;
        case ShaderDataType::Int3:      return 4 * 3;
        case ShaderDataType::Int4:      return 4 * 4;
        case ShaderDataType::Bool:      return 1;
        }

        BZ_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    uint32_t BufferElement::GetComponentCount() const
    {
        switch (Type)
        {
        case ShaderDataType::Float:     return 1;
        case ShaderDataType::Float2:    return 2;
        case ShaderDataType::Float3:    return 3;
        case ShaderDataType::Float4:    return 4;
        case ShaderDataType::Mat3:      return 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4;
        case ShaderDataType::Int:       return 1;
        case ShaderDataType::Int2:      return 2;
        case ShaderDataType::Int3:      return 3;
        case ShaderDataType::Int4:      return 4;
        case ShaderDataType::Bool:      return 1;
        }

        BZ_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    VertexBuffer* VertexBuffer::Create(const float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}