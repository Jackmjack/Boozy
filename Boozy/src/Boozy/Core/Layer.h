#pragma once
#include "Boozy/Core/Core.h"
#include "Boozy/Events/Event.h"
#include "Boozy/Core/Timestep.h"

#include <string>

namespace Boozy {

    class BOOZY_API Layer
    {
    public:
        Layer(const std::string& debugName = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep delta) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }
    private:
        std::string m_DebugName;
    };
}
