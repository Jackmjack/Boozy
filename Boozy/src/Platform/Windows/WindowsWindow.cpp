#include "bzpch.h"
#include "WindowsWindow.h"
#include "Boozy/Log.h"

#include "Boozy/Events/ApplicationEvent.h"
#include "Boozy/Events/KeyEvent.h"
#include "Boozy/Events/MouseEvent.h"


BZ_INIT_LOGGER("Window");

namespace Boozy {

    // 保证 GLFW 只初始化一次
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        BZ_ERROR("GLFW Error ({}): {}", error, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        glfwSetErrorCallback(GLFWErrorCallback);

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        BZ_INFO("Creating window: title = '{}', width = {}, height = {}", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            BZ_ASSERT(success, "Failed to initialize GLFW!");
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        BZ_ASSERT(m_Window, "Failed to create window!");

        m_Context = GraphicsContext::Create(this);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // 设置 GLFW 回调
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.Width = static_cast<unsigned int>(width);
                data.Height = static_cast<unsigned int>(height);

                WindowResizeEvent event(data.Width, data.Height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent event(key, 0);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(key);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressedEvent event(key, 1);
                        data.EventCallback(event);
                        break;
                    }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                KeyTypedEvent event(keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    void WindowsWindow::Shutdown()
    {
        delete m_Context;
        m_Context = nullptr;
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();           // 处理输入/窗口事件（触发各种回调）
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
