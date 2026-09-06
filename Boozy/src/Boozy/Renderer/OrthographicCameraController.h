#pragma once

#include "Boozy/Renderer/OrthographicCamera.h"
#include "Boozy/Core/Timestep.h"
#include "Boozy/Events/ApplicationEvent.h"
#include "Boozy/Events/MouseEvent.h"

namespace Boozy {

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool enableRotation = false);

        void OnUpdate(Timestep delta);
        void OnEvent(Event& event);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& event);
        bool OnWindowResized(WindowResizeEvent& event);

        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        bool m_EnableRotation;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;

        float m_CameraTranslationSpeed = 1.0f;
        float m_CameraRotationSpeed = 180.0f;
    };
}