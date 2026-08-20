#pragma once

#include "Boozy/Window.h"
#include <GLFW/glfw3.h>

#include "Boozy/Renderer/GraphicsContext.h"

namespace Boozy {

    /// \brief Windows平台窗口类
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        inline virtual void* GetNativeWindow() const override { return m_Window; }
        inline virtual GraphicsContext* GetContext() const override { return m_Context; }
    private:
        // 初始化函数，由构造函数调用
        void Init(const WindowProps& props);
        // 关闭函数，由析构函数调用
        void Shutdown();

        GLFWwindow* m_Window; // GLFW 窗口句柄
        GraphicsContext* m_Context = nullptr; // 图形上下文

        // 窗口数据结构体，方便挂载到 GLFW 窗口上
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback; // 事件回调：GLFW 事件触发时，用它把事件抛回应用层
        };

        WindowData m_Data;
    };
}
