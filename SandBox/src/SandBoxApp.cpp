#include <Boozy.h>

class SandBox : public Boozy::Application
{
public:
    SandBox()
    {
        Boozy::Log::SetLevel(Boozy::LogLevel::Trace);

        int a = 5, b = 6;
        BZ_TRACE("a={},b={}", a, b);
        BZ_DEBUG("a={},b={}", a, b);
        BZ_INFO("a={},b={}", a, b);
        BZ_WARN("a={},b={}", a, b);
        BZ_ERROR("a={},b={}", a, b);
        BZ_CRITICAL("a={},b={}", a, b);

        std::string msg = "出错了";
        BZ_TRACE(msg);
        BZ_DEBUG(msg);
        BZ_INFO(msg);
        BZ_WARN(msg);
        BZ_ERROR(msg);
        BZ_CRITICAL(msg);
    }

    ~SandBox()
    {

    }

};

Boozy::Application* Boozy::CreateApplication()
{
    return new SandBox();
}