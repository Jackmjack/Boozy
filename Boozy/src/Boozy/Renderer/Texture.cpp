#include "bzpch.h"
#include "Boozy/Renderer/Texture.h"

#include "Boozy/Core/Log.h"
#include "Boozy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(width, height);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:     BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(path);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<TextureCube> TextureCube::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTextureCube>(size);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<TextureCube> TextureCube::Create(const std::vector<std::string>& paths)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     BZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTextureCube>(paths);
        }

        BZ_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
