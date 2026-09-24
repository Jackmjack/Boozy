#pragma once
#include "Boozy/Core/Layer.h"
#include "Boozy/Events/Event.h"
#include "Boozy/Events/KeyEvent.h"
#include "Boozy/Events/MouseEvent.h"
#include "Boozy/Events/ApplicationEvent.h"

#include <vector>
#include <utility>

namespace Boozy {

    enum class DockSlot
    {
        Left = 0,
        Right,
        Down,
    };

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

        void BeginDockSpace();
        void EndDockSpace();

        void RegisterPanel(const std::string& name, DockSlot defaultSlot);
    private:
        void BuildDefaultLayout(uint32_t dockspaceID);

        float m_Time = 0.0f;

        bool m_LayoutBuilt = false;

        std::vector<std::pair<std::string, DockSlot>> m_Panels;
    };

}
