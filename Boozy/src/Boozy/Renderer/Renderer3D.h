#pragma once

#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/PerspectiveCamera.h"
#include "Boozy/Core/Transform.h"
#include "Boozy/Renderer/Mesh.h"
#include "Boozy/Renderer/Shader.h"
#include "Boozy/Renderer/Light.h"
#include "Boozy/Renderer/Material.h"

#include <glm/glm.hpp>

namespace Boozy
{

    class Renderer3D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera);
        static void BeginScene(const Camera& camera, const DirectionalLight& light);
        static void EndScene();

        static void DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Shader>& shader);
        static void DrawMesh(const Transform& transform, const Ref<Mesh>& mesh, const Ref<Material>& material);
    };

}
