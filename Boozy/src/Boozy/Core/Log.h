#pragma once
#include "Boozy/Core/Core.h"
#include <string>
#include <format>
#include <fstream>
#include <utility>

namespace Boozy {

    // 日志等级：数值越小越详细，越大越严重
    enum class LogLevel
    {
        Trace = 0,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    // 命名日志器：独立等级 + 独立日志文件
    class BOOZY_API Logger
    {
    public:
        static Logger& Register(const std::string& name);   // 注册日志器（同名复用）
        void SetLevel(LogLevel level) { m_Level = level; }  // 设置最低输出等级

        // 各等级便捷入口：std::format 拼好正文后交给 Write
        template <typename... Args>
        void Trace(std::format_string<Args...> fmt, Args&&... args)    { Write(LogLevel::Trace, std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        void Debug(std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Debug, std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        void Info(std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        void Warn(const std::string& file, std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Warn, file + std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        void Error(const std::string& file, std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Error, file + std::format(fmt, std::forward<Args>(args)...)); }
        template <typename... Args>
        void Critical(const std::string& file, std::format_string<Args...> fmt, Args&&... args) { Write(LogLevel::Critical, file + std::format(fmt, std::forward<Args>(args)...)); }

        // 各等级便捷入口：直接接收 string
        void Trace(const std::string& msg) { Write(LogLevel::Trace, msg); }
        void Debug(const std::string& msg) { Write(LogLevel::Debug, msg); }
        void Info(const std::string& msg) { Write(LogLevel::Info, msg); }
        void Warn(const std::string& file, const std::string& msg) { Write(LogLevel::Warn, file + msg); }
        void Error(const std::string& file, const std::string& msg) { Write(LogLevel::Error, file + msg); }
        void Critical(const std::string& file, const std::string& msg) { Write(LogLevel::Critical, file + msg); }

    private:
        Logger(const std::string& name); // 私有：只能通过 Register 创建
        void Write(LogLevel level, const std::string& message);

        std::string m_Name;
        LogLevel m_Level = LogLevel::Trace; // 最低过滤等级
        std::ofstream m_File;
    };

} // namespace Boozy

// =========================================
// 宏封装
// =========================================
// 每个需要日志的 .cpp 顶部调用一次：注册并绑定本文件的 logger
#define BZ_INIT_LOGGER(name) \
    static ::Boozy::Logger& BZ_LOGGER = ::Boozy::Logger::Register(name)

#define BZ_LOC std::string(__FILE__) + ":" + std::to_string(__LINE__) + " "

#define BZ_TRACE(...)    BZ_LOGGER.Trace(__VA_ARGS__)
#define BZ_DEBUG(...)    BZ_LOGGER.Debug(__VA_ARGS__)
#define BZ_INFO(...)     BZ_LOGGER.Info(__VA_ARGS__)
#define BZ_WARN(...)     BZ_LOGGER.Warn(BZ_LOC, __VA_ARGS__)
#define BZ_ERROR(...)    BZ_LOGGER.Error(BZ_LOC, __VA_ARGS__)
#define BZ_CRITICAL(...) BZ_LOGGER.Critical(BZ_LOC, __VA_ARGS__)
