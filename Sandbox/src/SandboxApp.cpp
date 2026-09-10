#include <Boozy.h>
#include <Boozy/Core/EntryPoint.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "Sandbox3D.h"

BZ_INIT_LOGGER("Sandbox"); // 初始化本文件 Sandbox 日志器

class Sandbox : public Boozy::Application
{
public:
    Sandbox()
    {
        //PushLayer(new Sandbox2D());
        PushLayer(new Sandbox3D());
    }

    ~Sandbox()
    {

    }

};

Boozy::Application* Boozy::CreateApplication()
{
    return new Sandbox();
}