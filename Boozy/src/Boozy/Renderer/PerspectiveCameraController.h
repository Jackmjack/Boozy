#pragma once

#include "Boozy/Renderer/PerspectiveCamera.h"
#include "Boozy/Core/Timestep.h"
#include "Boozy/Events/ApplicationEvent.h"
#include "Boozy/Events/MouseEvent.h"

#include <utility>

namespace Boozy {

    class PerspectiveCameraController
    {
    public:
        PerspectiveCameraController(float fovY = 45.0f, float aspect = 1.778f, float nearZ = 0.1f, float farZ = 100.0f);

        void OnUpdate(Timestep delta);
        void OnEvent(Event& event);

        PerspectiveCamera& GetCamera() { return m_Camera; }
        const PerspectiveCamera& GetCamera() const { return m_Camera; }
    private:
        bool OnWindowResized(WindowResizeEvent& event);

        float m_FovY;
        float m_Aspect;
        float m_NearZ;
        float m_FarZ;

        PerspectiveCamera m_Camera;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

        float m_CameraTranslationSpeed = 3.0f;
        float m_MouseSensitivity = 0.1f;
        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;
        std::pair<float, float> m_LastMousePosition = { 0.0f, 0.0f };

        bool m_FirstMouse = true;
    };
}