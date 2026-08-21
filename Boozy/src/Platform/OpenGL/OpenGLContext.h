#pragma once

#include "Boozy/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Boozy {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
        virtual void* GetCurrentContext() const override;
        virtual void MakeCurrentContext(void* context) override;

    private:
        GLFWwindow* m_WindowHandle;
    };

}


