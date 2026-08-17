#include "bzpch.h"
#include "Log.h"
#include <memory>
#include <unordered_map>

namespace Boozy {

    // 线程锁
    static std::mutex g_LogMutex;

    // 每个等级对应的控制台前景色
    static WORD LevelToColor(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:    return FOREGROUND_INTENSITY;                                     // 灰
        case LogLevel::Debug:    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;      // 白
        case LogLevel::Info:     return FOREGROUND_GREEN | FOREGROUND_INTENSITY;                  // 亮绿
        case LogLevel::Warn:     return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // 亮黄
        case LogLevel::Error:    return FOREGROUND_RED | FOREGROUND_INTENSITY;                    // 亮红
        case LogLevel::Critical: return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;  // 亮紫
        default:                 return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;      // 默认白
        }
    }

    // 等级名称，补成 5 字符宽让前缀对齐
    static const char* LevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO ";
        case LogLevel::Warn:     return "WARN ";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRIT ";
        default:                 return "     ";
        }
    }

    // 当前时间，格式 HH:MM:SS
    static std::string CurrentTime()
    {
        std::time_t t = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &t);
        std::ostringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");
        return ss.str();
    }

    // 获取 exe 所在目录
    static std::filesystem::path GetExecutableDirectory()
    {
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        return std::filesystem::path(buffer).parent_path();
    }

    Logger::Logger(const std::string& name)
        : m_Name(name)
    {
        // 统一写入 exe 旁 logs/ 目录
        std::filesystem::path logDir = GetExecutableDirectory() / "logs";
        std::error_code ec;
        std::filesystem::create_directories(logDir, ec);
        m_File.open(logDir / (name + ".log"), std::ios::app);
    }

    void Logger::Write(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(g_LogMutex);

        if (level < m_Level)
            return;

        // [时间][等级][模块]:正文
        std::string line = "[" + CurrentTime() + "][" + LevelToString(level) + "][" + m_Name + "] " + message;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole)
            SetConsoleTextAttribute(hConsole, LevelToColor(level));
        std::cout << line << std::endl;

        // 恢复默认色
        if (hConsole)
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        // 写入本模块的日志文件
        if (m_File)
            m_File << line << std::endl;
    }

    // 注册表用函数局部 static（magic static）懒初始化：首次调用才构造
    static std::unordered_map<std::string, std::unique_ptr<Logger>>& GetLoggers()
    {
        static std::unordered_map<std::string, std::unique_ptr<Logger>> s_Loggers;
        return s_Loggers;
    }

    Logger& Logger::Register(const std::string& name)
    {
        auto& loggers = GetLoggers();
        auto it = loggers.find(name);
        if (it != loggers.end())
            return *it->second;

        // 未注册则创建并打开该模块的日志文件
        std::unique_ptr<Logger> logger(new Logger(name));
        Logger& ref = *logger;
        loggers.emplace(name, std::move(logger));
        return ref;
    }

}
