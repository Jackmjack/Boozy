#pragma once
#include "Boozy/Core/Core.h"
#include "Boozy/Core/Window.h"
#include "Boozy/Events/ApplicationEvent.h"
#include "Boozy/Core/LayerStack.h"
#include "Boozy/ImGui/ImGuiLayer.h"
#include "Boozy/Core/Timestep.h"
#include <memory>

namespace Boozy {

    class BOOZY_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }
        ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }

        inline static Application& GetInstance() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        double m_LastFrameTime = 0.0f;

        static Application* s_Instance;
    };

    // 在客户端中定义
    Application* CreateApplication();

}