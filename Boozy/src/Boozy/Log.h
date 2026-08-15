#pragma once
#include "Core.h"
#include <string>
#include <format>
#include <utility>

namespace Boozy {

    /// \brief 日志等级：数值越小越详细，越大越严重
    enum class LogLevel
    {
        Trace = 0, ///< 详细跟踪信息
        Debug,     ///< 普通调试信息
        Info,      ///< 常规运行信息
        Warn,      ///< 警告信息
        Error,     ///< 错误信息
        Critical   ///< 严重错误信息
    };

    /// \brief 全局日志器：静态工具类，无需实例化
    class BOOZY_API Log
    {
    public:
        /// \brief 设置最低输出等级
        /// \param level 新的最低输出等级
        static void SetLevel(LogLevel level);

        /// \brief 获取当前输出等级
        /// \return 当前的最低输出等级
        static LogLevel GetLevel();

        /// \brief 核心输出底层接口
        /// \param level 日志等级
        /// \param message 日志信息
        static void Write(LogLevel level, const std::string& message);

        // 各等级便捷入口：用 std::format 拼好正文后交给 Write
        template <typename... Args>
        static void Trace(std::format_string<Args...> fmt, Args&&... args)    { Write(LogLevel::Trace, std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        static void Debug(std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Debug, std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        static void Info(std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        static void Warn(const std::string& file, std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Warn, file + std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        static void Error(const std::string& file, std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Error, file + std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        static void Critical(const std::string& file, std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Critical, file + std::format(fmt, std::forward<Args>(args)...)); }

        // 各等级便捷入口：函数重载，直接接收string
        static void Trace(const std::string& msg) { Write(LogLevel::Trace, msg); }
        static void Debug(const std::string& msg) { Write(LogLevel::Debug, msg); }
        static void Info(const std::string& msg) { Write(LogLevel::Info, msg); }
        static void Warn(const std::string& file, const std::string& msg) { Write(LogLevel::Warn, file + msg); }
        static void Error(const std::string& file, const std::string& msg) { Write(LogLevel::Error, file + msg); }
        static void Critical(const std::string& file, const std::string& msg) { Write(LogLevel::Critical, file + msg); }

    private:
        static LogLevel s_Level; // 全局最低过滤等级
    };
} // namespace Boozy

// =========================================
// 宏封装
// =========================================
#define BZ_TRACE(...)    ::Boozy::Log::Trace(__VA_ARGS__)
#define BZ_DEBUG(...)    ::Boozy::Log::Debug(__VA_ARGS__)
#define BZ_INFO(...)     ::Boozy::Log::Info(__VA_ARGS__)
#define BZ_WARN(...)     ::Boozy::Log::Warn(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " ", __VA_ARGS__)
#define BZ_ERROR(...)    ::Boozy::Log::Error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " ", __VA_ARGS__)
#define BZ_CRITICAL(...) ::Boozy::Log::Critical(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " ", __VA_ARGS__)
