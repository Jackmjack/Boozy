#pragma once

#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/PerspectiveCamera.h"
#include "Boozy/Core/Transform.h"
#include "Boozy/Renderer/Mesh.h"
#include "Boozy/Renderer/Shader.h"
#include "Boozy/Renderer/Light.h"
#include "Boozy/Renderer/Material.h"

#include <vector>
#include <glm/glm.hpp>

namespace Boozy
{

    class Renderer3D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera);
        static void BeginScene(const Camera& camera, const std::vector<Light>& lights, const glm::vec3 ambient = { 0.1f,0.1f,0.1f });
        static void EndScene();

        static void BeginShadow();
        static void DrawShadow(const Transform& transform, const Ref<Mesh>& mesh);
        static void EndShadow();

        static void DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Shader>& shader);
        static void DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Material>& material);

        static void DrawFloor(const Ref<Shader>& shader);
        static void DrawSkybox(const Ref<TextureCube>& skybox, const Ref<Shader>& shader);
    };

}
