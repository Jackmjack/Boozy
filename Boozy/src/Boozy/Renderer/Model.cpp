#include "bzpch.h"
#include "Model.h"

#include "Boozy/Core/Log.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy {

    static const BufferLayout layout = {
        { Boozy::ShaderDataType::Float3, "a_Position" },
        { Boozy::ShaderDataType::Float3, "a_Normal" },
        { Boozy::ShaderDataType::Float2, "a_UV" }
    };

    namespace {

        void ProcessMesh(const aiMesh* mesh, const aiScene* scene, std::vector<MeshEntry>& out)
        {
            std::vector<Vertex> vertices;

            for (unsigned i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex v{};

                v.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

                if (mesh->HasNormals())
                    v.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

                if (mesh->HasTextureCoords(0))
                    v.UV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

                vertices.push_back(v);
            }

            std::vector<uint32_t> indices;

            for (unsigned i = 0; i < mesh->mNumFaces; i++)
            {
                const aiFace& face = mesh->mFaces[i];
                for (unsigned j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            Ref<Mesh> m;
            m.reset(new Mesh(vertices.data(), (uint32_t)vertices.size(), indices.data(), (uint32_t)indices.size(), layout));

            glm::vec3 mn(std::numeric_limits<float>::max());
            glm::vec3 mx(std::numeric_limits<float>::lowest());
            for (const Vertex& v : vertices)
            {
                mn = glm::min(mn, v.Position);
                mx = glm::max(mx, v.Position);
            }
            const glm::vec3 center = (mn + mx) * 0.5f;
            float radius = 0.0f;
            for (const Vertex& v : vertices)
                radius = glm::max(radius, glm::length(v.Position - center));

            out.push_back({ m, mesh->mMaterialIndex, mn, mx, center, radius });
        }

        void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<MeshEntry>& out)
        {
            for (unsigned i = 0; i < node->mNumMeshes; i++)
                ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, out);

            for (unsigned i = 0; i < node->mNumChildren; i++)
                ProcessNode(node->mChildren[i], scene, out);
        }
    }

    Model::Model(const std::string& path)
        : m_Path(path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
        {
            BZ_ERROR("Failed to load model: {} ({})", path, importer.GetErrorString());
            return;
        }

        ProcessNode(scene->mRootNode, scene, m_Entries);

        BZ_INFO("Loaded model: {} ({} sub-mesh(es))", path, m_Entries.size());
    }

}