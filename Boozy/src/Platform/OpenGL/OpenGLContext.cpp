#include "bzpch.h"
#include "OpenGLContext.h"
#include "Boozy/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

BZ_INIT_LOGGER("OpenGL");

namespace Boozy {

    GraphicsContext* GraphicsContext::Create(Window* window)
    {
        return new OpenGLContext(static_cast<GLFWwindow*>(window->GetNativeWindow()));
    }

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        BZ_ASSERT(m_WindowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BZ_ASSERT(status, "Failed to initialize Glad!");
        BZ_INFO("Initialized OpenGL context successfully.");
        BZ_INFO("Vendor:   {}", (const char*)glGetString(GL_VENDOR));
        BZ_INFO("Renderer: {}", (const char*)glGetString(GL_RENDERER));
        BZ_INFO("Version:  {}", (const char*)glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

    void* OpenGLContext::GetCurrentContext() const
    {
        return glfwGetCurrentContext();
    }

    void OpenGLContext::MakeCurrentContext(void* context)
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(context));
    }
}
