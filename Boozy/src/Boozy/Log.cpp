#include "bzpch.h"
#include "Log.h"

namespace Boozy {

    // 默认全开，方便调试
    LogLevel Log::s_Level = LogLevel::Trace;

    // 线程锁
    static std::mutex g_LogMutex;
    // 日志文件
    static std::ofstream g_LogFile("boozy.log", std::ios::app);

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

    // =======================================================
    // 公开接口实现
    // =======================================================
    void Log::SetLevel(LogLevel level) { std::lock_guard<std::mutex> lock(g_LogMutex); s_Level = level; }
    LogLevel Log::GetLevel() { std::lock_guard<std::mutex> lock(g_LogMutex); return s_Level; }

    void Log::Write(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(g_LogMutex);

        if (level < s_Level)
            return;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        std::string time = CurrentTime();

        // [时间][等级]:正文
        if (hConsole)
            SetConsoleTextAttribute(hConsole, LevelToColor(level));
        std::cout << "[" << time << "][" << LevelToString(level) << "] " << message << std::endl;

        // 恢复默认色
        if (hConsole)
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        // 写入到日志文件
        if (g_LogFile)
            g_LogFile << "[" << time << "][" << LevelToString(level) << "] " << message << std::endl;
    }
} // namespace Boozy
