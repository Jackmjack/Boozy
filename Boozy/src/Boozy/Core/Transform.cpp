#include "bzpch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Boozy
{
    Transform::Transform()
        : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f)
    {
        UpdateMatrix();
    }

    Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        : m_Position(position), m_Rotation(rotation), m_Scale(scale)
    {
        UpdateMatrix();
    }

    void Transform::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        UpdateMatrix();
    }

    void Transform::SetRotation(const glm::vec3& rotation)
    {
        m_Rotation = rotation;
        UpdateMatrix();
    }

    void Transform::SetScale(const glm::vec3& scale)
    {
        m_Scale = scale;
        UpdateMatrix();
    }

    void Transform::UpdateMatrix()
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                           * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
                           * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
        m_TransformMatrix = translation * rotation * scale;
    }
}