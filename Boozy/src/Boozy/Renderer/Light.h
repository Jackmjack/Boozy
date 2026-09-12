#pragma once
#include <glm/glm.hpp>

namespace Boozy {

    struct DirectionalLight
    {
        glm::vec3 Direction = { -0.5f, -1.0f, -0.3f }; // 从光源指向物体
        glm::vec3 Color = { 1.0f,1.0f,1.0f };
        float Intensity = 1.0f;
        glm::vec3 Ambient = { 0.1f,0.1f,0.1f };
    };

}