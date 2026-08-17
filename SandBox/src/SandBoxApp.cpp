#include <Boozy.h>

BZ_INIT_LOGGER("SandBox"); // 初始化本文件 SandBox 日志器

class ExampleLayer : public Boozy::Layer
{
public:
    ExampleLayer()
        : Layer("Example") {}

    void OnUpdate() override
    {
        //BZ_INFO("ExampleLayer::Update");
    }

    void OnEvent(Boozy::Event& event) override
    {
        BZ_TRACE(event);
    }
};

class SandBox : public Boozy::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }

    ~SandBox()
    {

    }

};

Boozy::Application* Boozy::CreateApplication()
{
    return new SandBox();
}