#pragma once

extern Boozy::Application* Boozy::CreateApplication();

int main(int argc, char** argv)
{
    auto app = Boozy::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
