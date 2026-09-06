#pragma once
#include "Boozy/Core/Window.h"

namespace Boozy {

    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void* GetCurrentContext() const = 0;
        virtual void MakeCurrentContext(void* context) = 0;

        static GraphicsContext* Create(Window* window);
    };
}