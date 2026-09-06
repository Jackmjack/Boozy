#pragma once
#include "Boozy/Core/Layer.h"
#include "Boozy/Events/Event.h"
#include "Boozy/Events/KeyEvent.h"
#include "Boozy/Events/MouseEvent.h"
#include "Boozy/Events/ApplicationEvent.h"

namespace Boozy {

    class BOOZY_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
    };

}
