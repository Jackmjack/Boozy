#include "bzpch.h"
#include "Boozy/Renderer/Texture.h"

#include "Boozy/Core/Log.h"
#include "Boozy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {

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


}
