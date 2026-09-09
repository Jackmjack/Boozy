#include "bzpch.h"
#include "Boozy/Core/Application.h"
#include "Boozy/Core/Log.h"
#include "Boozy/Renderer/Renderer.h"

BZ_INIT_LOGGER("Application");

namespace Boozy {

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        BZ_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BZ_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
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
        dispatcher.Dispatch<WindowResizeEvent>(BZ_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.GetHandled())
                break;
        }
    }

    void Application::Run()
    {
        m_LastFrameTime = m_Window->GetTime();

        while (m_Running)
        {
            double time = m_Window->GetTime();
            Timestep delta = (float)(time - m_LastFrameTime);
            m_LastFrameTime = time;

            if (delta.GetSeconds() > 0.04)
                delta = 0.04f;

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(delta);
            }

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

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;

        Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

        return false;
    }
}
