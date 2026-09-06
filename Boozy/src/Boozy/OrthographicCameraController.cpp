#include "bzpch.h"
#include "OrthographicCameraController.h"

#include "Boozy/Input.h"
#include "Boozy/KeyCodes.h"

namespace Boozy {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool enableRotation)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_EnableRotation(enableRotation)
    {}

    void OrthographicCameraController::OnUpdate(Timestep delta)
    {
        if (Boozy::Input::IsKeyPressed(BZ_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * delta;

        if (m_EnableRotation)
        {
            if (Boozy::Input::IsKeyPressed(BZ_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * delta;

            if (Boozy::Input::IsKeyPressed(BZ_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * delta;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);
    }

    void Boozy::OrthographicCameraController::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>(BZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
    {
        m_ZoomLevel -= event.GetYOffset() * 0.25f;
        m_ZoomLevel = std::clamp(m_ZoomLevel, 0.25f, 10.0f);
        m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        m_CameraTranslationSpeed = m_ZoomLevel;
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
    {
        if (event.GetHeight() == 0)
            return false;

        m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
        m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}