#pragma once
#include "Boozy/Core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

        static std::vector<glm::mat4> MakePointShadowMatrix(const Light& point)
        {
            const glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 50.0f);

            // 六个朝向，顺序必须严格对应 GL 的 face 枚举
            const glm::vec3 dirs[6] = {
                { 1, 0, 0}, {-1, 0, 0},
                { 0, 1, 0}, { 0,-1, 0},
                { 0, 0, 1}, { 0, 0,-1},
            };
            const glm::vec3 ups[6] = {
                {0,-1, 0}, {0,-1, 0},      // +X / -X
                {0, 0, 1}, {0, 0,-1},      // +Y / -Y
                {0,-1, 0}, {0,-1, 0},      // +Z / -Z
            };

            std::vector<glm::mat4> lightSpace;
            lightSpace.resize(6);
            for (int f = 0; f < 6; f++) {
                lightSpace[f] = proj * glm::lookAt(point.Position, point.Position + dirs[f], ups[f]);
            }

            return lightSpace;
        }

        static glm::mat4 MakeSpotShadowMatrix(const Light& spot)
        {
            if (spot.Type != LightType::Spot) {
                return glm::mat4(1.0f);
            }

            glm::vec3 up = std::abs(spot.Direction.y) > 0.99f ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
            glm::mat4 spotView = glm::lookAt(spot.Position, spot.Position + spot.Direction, up);
            glm::mat4 spotProj = glm::perspective(glm::radians(spot.OuterCutOff * 2), 1.0f, 0.1f, 100.0f);
            return spotProj * spotView;
        }
    };

}