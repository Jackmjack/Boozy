#include "bzpch.h"
#include "OpenGLContext.h"
#include "Boozy/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

BZ_INIT_LOGGER("Core"); // 初始化本文件（引擎窗口模块）日志器

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
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

    void OpenGLContext::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a); // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清屏
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
