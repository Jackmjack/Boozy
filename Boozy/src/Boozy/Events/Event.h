#pragma once
#include "Boozy/Core.h"
#include <string>
#include <functional>
#include <ostream>

namespace Boozy {

    /// \brief 事件类型枚举
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    /// \brief 事件种类枚举：采用掩码的方式，获取种类直接按位与
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0), ///< 掩码：00001
        EventCategoryInput       = BIT(1), ///< 掩码：00010
        EventCategoryKeyboard    = BIT(2), ///< 掩码：00100
        EventCategoryMouse       = BIT(3), ///< 掩码：01000
        EventCategoryMouseButton = BIT(4)  ///< 掩码：10000
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    /// \brief 事件基类
    class BOOZY_API Event
    {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }
        operator std::string() const { return ToString(); }
        bool GetHandled() const { return m_Handled; }

        inline bool IsInCategory(EventCategory category) const
        {
            return GetCategoryFlags() & category;
        }
    protected:
        // 事件是否被处理
        bool m_Handled = false;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event)
            : m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.m_Handled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}