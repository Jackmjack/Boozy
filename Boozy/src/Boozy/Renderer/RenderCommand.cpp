#include "bzpch.h"
#include "Boozy/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Boozy {

    static OpenGLRendererAPI s_OpenGLRendererAPI;
    RendererAPI* RenderCommand::s_RendererAPI = &s_OpenGLRendererAPI;

}
