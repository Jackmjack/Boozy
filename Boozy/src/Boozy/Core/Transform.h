#pragma once

#include <glm/glm.hpp>

namespace Boozy
{

    class Transform
    {
    public:
        Transform();
        Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& rotation);
        void SetScale(const glm::vec3& scale);

        glm::vec3 GetPosition() const { return m_Position; }
        glm::vec3 GetRotation() const { return m_Rotation; }
        glm::vec3 GetScale() const { return m_Scale; }

        glm::mat4 GetTransformMatrix() const { return m_TransformMatrix; }

    private:
        void UpdateMatrix();

        glm::vec3 m_Position;
        glm::vec3 m_Rotation; // in degrees
        glm::vec3 m_Scale;

        glm::mat4 m_TransformMatrix;
    };

}
