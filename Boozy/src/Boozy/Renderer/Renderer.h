#pragma once
#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/RenderCommand.h"

#include <cstdint>

namespace Boozy {

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void OnWindowResize(uint32_t width, uint32_t height);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}
