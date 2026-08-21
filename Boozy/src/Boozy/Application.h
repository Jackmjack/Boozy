#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include <memory>


#include <Boozy/Renderer/Shader.h>
#include <Boozy/Renderer/Buffer.h>
#include <Boozy/Renderer/VertexArray.h>

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

        inline static Application& GetInstance() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& event);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        static Application* s_Instance;
    };

    // 在客户端中定义
    Application* CreateApplication();

}