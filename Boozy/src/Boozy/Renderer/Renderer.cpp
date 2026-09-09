#include "bzpch.h"
#include "Boozy/Renderer/Renderer.h"
#include "Boozy/Renderer/Renderer2D.h"

namespace Boozy {

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

}
