#pragma once

#include "Boozy/Core/Core.h"
#include "Boozy/Core/Transform.h"
#include "Boozy/Renderer/Model.h"
#include "Boozy/Renderer/Material.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Boozy
{
    struct SceneObject
    {
        uint32_t ID = 0;
        std::string Name;

        Ref<Model> Model;
        Ref<Material> Material;

        Transform ObTransform;

        bool Visible = true;
    };

    class Scene
    {
    public:
        Scene() = default;

        SceneObject& Add(const Ref<Model>& model, const Ref<Material>& material,
            const Transform& obTransform = Transform(),
            const std::string& name = "Object");

        SceneObject* Find(uint32_t id);

        bool Remove(uint32_t id);
        void Clear();

        std::vector<SceneObject>& GetObjects() { return m_Objects; }
        const std::vector<SceneObject>& GetObjects() const { return m_Objects; }

    private:
        std::vector<SceneObject> m_Objects;
        uint32_t m_NextID = 1;
    };
}
