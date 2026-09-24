#include "bzpch.h"
#include "PerspectiveCameraController.h"

#include "Boozy/Core/Input.h"
#include "Boozy/Core/KeyCodes.h"
#include "Boozy/Core/MouseButtonCodes.h"

namespace Boozy {

    PerspectiveCameraController::PerspectiveCameraController(float fovY, float aspect, float nearZ, float farZ)
        : m_FovY(fovY), m_Aspect(aspect), m_NearZ(nearZ), m_FarZ(farZ), m_Camera(m_FovY, m_Aspect, m_NearZ, m_FarZ)
    {}

    void PerspectiveCameraController::OnUpdate(Timestep delta)
    {
        if (Input::IsMouseButtonPressed(BZ_MOUSE_BUTTON_1))
        {
            if (m_FirstMouse) { m_LastMousePosition = Input::GetMousePosition(); m_FirstMouse = false; }

            float MouseX = Input::GetMouseX();
            float MouseY = Input::GetMouseY();

            float deltaX = MouseX - m_LastMousePosition.first;
            float deltaY = MouseY - m_LastMousePosition.second;

            m_LastMousePosition = { MouseX, MouseY };

            m_Yaw += deltaX * m_MouseSensitivity;
            m_Pitch -= deltaY * m_MouseSensitivity;

            m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
        }
        else m_FirstMouse = true;

        const float yaw = glm::radians(m_Yaw);
        const float pitch = glm::radians(m_Pitch);
        glm::vec3 forward;
        forward.x = std::cos(pitch) * std::cos(yaw);
        forward.y = std::sin(pitch);
        forward.z = std::cos(pitch) * std::sin(yaw);

        const glm::vec3 forwardFlat{ std::cos(yaw), 0.0f, std::sin(yaw) };
        const glm::vec3 rightFlat{ -std::sin(yaw), 0.0f, std::cos(yaw) };

        m_CameraPosition += forwardFlat * 
            ((Boozy::Input::IsKeyPressed(BZ_KEY_W) ? 1.0f : 0.0f) - (Boozy::Input::IsKeyPressed(BZ_KEY_S) ? 1.0f : 0.0f)) * 
            m_CameraTranslationSpeed * (float)delta;

        m_CameraPosition += rightFlat * 
            ((Boozy::Input::IsKeyPressed(BZ_KEY_D) ? 1.0f : 0.0f) - (Boozy::Input::IsKeyPressed(BZ_KEY_A) ? 1.0f : 0.0f)) * 
            m_CameraTranslationSpeed * (float)delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_SPACE))
            m_CameraPosition.y += m_CameraTranslationSpeed * delta;

        if (Boozy::Input::IsKeyPressed(BZ_KEY_LEFT_SHIFT))
            m_CameraPosition.y -= m_CameraTranslationSpeed * delta;

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetLookAt(m_CameraPosition + forward);
    }

    void PerspectiveCameraController::OnEvent(Event & event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>(BZ_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
    }

    bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& event)
    {
        if (event.GetHeight() == 0)
            return false;

        m_Aspect = (float)event.GetWidth() / (float)event.GetHeight();
        m_Camera.SetProjectionMatrix(m_FovY, m_Aspect, m_NearZ, m_FarZ);
        return false;
    }

}