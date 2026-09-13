#pragma once
#include <glm/glm.hpp>

namespace Boozy {
    inline constexpr int BZ_MAX_LIGHTS = 8;

    enum class LightType { Directional = 0, Point = 1, Spot = 2 };

    struct Light
    {
        LightType Type = LightType::Point;

        // Uniform
        glm::vec3 Color = { 1.0f,1.0f,1.0f };
        float Intensity = 1.0f;

        // Directional and Spot
        glm::vec3 Direction = { -0.5f, -1.0f, -0.3f }; // 从光源指向物体

        // Point and Spot
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };

        // Point
        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;

        // Spot
        float InnerCutOff = 12.5f;
        float OuterCutOff = 17.5f;

        static Light MakeDirectional(const glm::vec3& direction, const glm::vec3& color, float intensity = 1.0f)
        {
            Light light;
            light.Type = LightType::Directional;
            light.Direction = direction;
            light.Color = color;
            light.Intensity = intensity;
            return light;
        }

        static Light MakePoint(const glm::vec3& position, const glm::vec3& color, float intensity = 1.0f)
        {
            Light light;
            light.Type = LightType::Point;
            light.Position = position;
            light.Color = color;
            light.Intensity = intensity;
            return light;
        }

        static Light MakeSpot(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float innerDeg, float outerDeg, float intensity = 1.0f)
        {
            Light light;
            light.Type = LightType::Spot;
            light.Position = position;
            light.Direction = direction;
            light.Color = color;
            light.InnerCutOff = innerDeg;
            light.OuterCutOff = outerDeg;
            light.Intensity = intensity;
            return light;
        }
    };

}