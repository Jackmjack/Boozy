#include "bzpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include <GLFW/glfw3.h>

namespace Boozy {

    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {

        while (m_Running)
        {
            glClearColor(1, 0, 1, 1); // 设置清屏颜色 (R, G, B, A)
            glClear(GL_COLOR_BUFFER_BIT); // 清屏
            m_Window->OnUpdate();
        }
    }

}