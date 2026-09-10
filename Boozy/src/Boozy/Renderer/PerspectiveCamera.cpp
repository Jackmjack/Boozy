#include "bzpch.h"
#include "PerspectiveCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Boozy
{
    PerspectiveCamera::PerspectiveCamera(float fovY, float aspect, float nearZ, float farZ)
        : m_ProjectionMatrix(glm::perspective(glm::radians(fovY), aspect, nearZ, farZ))
    {
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetProjectionMatrix(float fovY, float aspect, float nearZ, float farZ)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(fovY), aspect, nearZ, farZ);
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::RecalculateViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0.0f, 1.0f, 0.0f));
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}