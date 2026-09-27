#pragma once

#include "Boozy/Core/Core.h"
#include "Boozy/Events/Event.h"
#include <string>
#include <functional>

namespace Boozy {

    class GraphicsContext;

    enum class WindowCursorMode {
        None = 0,
        Normal,
        Hidden,
        Disabled,
        Captured
    };

    /// \brief 窗口基本属性结构体
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        WindowCursorMode CursorMode;

        WindowProps(const std::string& title = "Boozy Engine",
            unsigned int width = 1280,
            unsigned int height = 720,
            WindowCursorMode mode = WindowCursorMode::Normal)
            : Title(title), Width(width), Height(height), CursorMode(mode) {}
    };

    /// \brief 窗口抽象
    class BOOZY_API Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        /// \brief 每帧更新：轮询事件 + 交换缓冲
        virtual void OnUpdate() = 0;

        virtual double GetTime() const = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        virtual void SetCursorMode(const WindowCursorMode mode) = 0;
        virtual WindowCursorMode GetCursorMode() const = 0;

        /// \brief 设置是否开启垂直同步
        /// \param enabled true 开启（锁定显示器刷新率），false 关闭（不限帧率）
        virtual void SetVSync(bool enabled) = 0;

        /// \brief 查询当前是否开启垂直同步
        virtual bool IsVSync() const = 0;

        /// \brief 获取窗口句柄
        virtual void* GetNativeWindow() const = 0;

        /// \brief 获取上下文
        virtual GraphicsContext* GetContext() const = 0;

        /// \brief 静态 Create 方法，应用层只调用接口的该方法，细节由子类实现
        static Window* Create(const WindowProps& props = WindowProps());
    };
}