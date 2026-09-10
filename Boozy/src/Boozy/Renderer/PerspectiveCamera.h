#pragma once

#include "Boozy/Renderer/Camera.h"
#include "glm/glm.hpp"

namespace Boozy
{

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fovY = 45.0f, float aspect = 1.778f, float nearZ = 0.1f, float farZ = 100.0f);

        void SetProjectionMatrix(float fovY, float aspect, float nearZ, float farZ);

        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        void SetLookAt(const glm::vec3& lookAt) { m_LookAt = lookAt; RecalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetLookAt() const { return m_LookAt; }

    private:
        void RecalculateViewMatrix();

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_LookAt = { 0.0f, 0.0f, -1.0f };
    };

}
