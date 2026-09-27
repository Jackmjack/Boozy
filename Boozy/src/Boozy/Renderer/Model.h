#pragma once

#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/Mesh.h"

#include <string>
#include <vector>

namespace Boozy {

    struct MeshEntry
    {
        Ref<Mesh> Mesh;
        uint32_t MaterialIndex = 0;

        glm::vec3 BoundsMin{ 0.0f };
        glm::vec3 BoundsMax{ 0.0f };
        glm::vec3 BoundsCenter{ 0.0f };
        float BoundsRadius = 1.0f;
    };

    class Model
    {
    public:
        Model(const std::string& path);

        const std::vector<MeshEntry>& GetMeshes() const { return m_Entries; }

        bool IsValid() const { return !m_Entries.empty(); }
        const std::string& GetPath() const { return m_Path; }
    private:
        std::string m_Path;

        std::vector<MeshEntry> m_Entries;
    };

}

