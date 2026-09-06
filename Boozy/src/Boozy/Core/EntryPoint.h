#pragma once

#ifdef BZ_PLATFORM_WINDOWS

extern Boozy::Application* Boozy::CreateApplication();

int main(int argc, char** argv)
{
    auto app = Boozy::CreateApplication();
    app->Run();
    delete app;
}

#endif