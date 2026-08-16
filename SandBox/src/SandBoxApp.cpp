#include <Boozy.h>

class SandBox : public Boozy::Application
{
public:
    SandBox()
    {

    }

    ~SandBox()
    {

    }

};

Boozy::Application* Boozy::CreateApplication()
{
    return new SandBox();
}