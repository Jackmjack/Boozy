#include <Boozy.h>
#include <imgui.h>

BZ_INIT_LOGGER("SandBox"); // 初始化本文件 SandBox 日志器

class ExampleLayer : public Boozy::Layer
{
public:
    ExampleLayer()
        : Layer("Example") {}

    void OnUpdate() override
    {
        
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello world");
        ImGui::End();
    }

    void OnEvent(Boozy::Event& event) override
    {
        if (event.GetEventType() == Boozy::EventType::KeyPressed)
        {
            Boozy::KeyPressedEvent& e = (Boozy::KeyPressedEvent&)event;
            if (e.GetKeyCode() == BZ_KEY_TAB)
                BZ_INFO("Tab key is pressed!");
            BZ_TRACE("{}", char(e.GetKeyCode()));

        }
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