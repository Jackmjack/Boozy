#include "bzpch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"
#include <glad/glad.h>

BZ_INIT_LOGGER("Core"); // 初始化本文件日志器

namespace Boozy {

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        BZ_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BZ_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }

    Application::~Application()
    {

    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BZ_BIND_EVENT_FN(Application::OnWindowClose));

        //BZ_TRACE(e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.GetHandled())
                break;
        }
    }

    void Application::Run()
    {

        while (m_Running)
        {
            glClearColor(1, 0, 1, 1); // 设置清屏颜色 (R, G, B, A)
            glClear(GL_COLOR_BUFFER_BIT); // 清屏

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }

}
