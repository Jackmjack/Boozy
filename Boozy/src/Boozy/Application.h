#pragma once
#include "Core.h"
#include "Window.h"
#include <memory>

namespace Boozy {

    class BOOZY_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // 在客户端中定义
    Application* CreateApplication();

}