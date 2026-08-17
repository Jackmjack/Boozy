#include <Boozy.h>

class ExampleLayer : public Boozy::Layer
{
public:
    ExampleLayer()
        : Layer("Example") {}

    void OnUpdate() override
    {
        BZ_INFO("ExampleLayer::Update");
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