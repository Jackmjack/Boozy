#pragma once

#include <glm/glm.hpp>

namespace Boozy {

    class Frustum
    {
    public:
        Frustum() = default;
        explicit Frustum(const glm::mat4& viewProjection);

        bool IsInside(const glm::vec3& center, float radius) const;
        bool IsInside(const glm::vec3& min, const glm::vec3& max) const;
    private:
        glm::vec4 m_Planes[6];
    };

}
