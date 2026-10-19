#include "bzpch.h"
#include "Boozy/Core/Log.h"

namespace Boozy {

    // 线程锁
    static std::mutex g_LogMutex;

    // 每个等级对应的控制台前景色
    #if defined(BZ_PLATFORM_WINDOWS)
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
    #elif defined(BZ_PLATFORM_LINUX)
    static const char* LevelToAnsi(LogLevel level) {
        switch (level) {
        case LogLevel::Trace:    return "\033[90m";   // 亮黑
        case LogLevel::Debug:    return "\033[37m";
        case LogLevel::Info:     return "\033[92m";
        case LogLevel::Warn:     return "\033[93m";
        case LogLevel::Error:    return "\033[91m";
        case LogLevel::Critical: return "\033[95m";
        default:                 return "\033[0m";
        }
    }
    #else
        #error Boozy: unsupported platform!
    #endif

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
        #if defined(BZ_PLATFORM_WINDOWS)
            localtime_s(&tm, &t);
        #elif defined(BZ_PLATFORM_LINUX)
            localtime_r(&t, &tm);
        #else
            #error Boozy: unsupported platform!
        #endif
        std::ostringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");
        return ss.str();
    }

    // 获取 exe 所在目录
    static std::filesystem::path GetExecutableDirectory()
    {
        #if defined(BZ_PLATFORM_WINDOWS)
            wchar_t buffer[MAX_PATH];
            GetModuleFileNameW(nullptr, buffer, MAX_PATH);
            return std::filesystem::path(buffer).parent_path();
        #elif defined(BZ_PLATFORM_LINUX)
            std::error_code ec;
            auto p = std::filesystem::read_symlink("/proc/self/exe", ec);
            return ec ? std::filesystem::current_path() : p.parent_path();
        #else
            #error Boozy: unsupported platform!
        #endif
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

        #if defined(BZ_PLATFORM_WINDOWS)
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsole)
                SetConsoleTextAttribute(hConsole, LevelToColor(level));
            std::cout << line << std::endl;

            // 恢复默认色
            if (hConsole)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        #elif defined(BZ_PLATFORM_LINUX)
            std::cout << LevelToAnsi(level) << line << "\033[0m" << std::endl;
        #else
            #error Boozy: unsupported platform!
        #endif

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
