#include "bzpch.h"
#include "Scene.h"

namespace Boozy
{
    SceneObject& Scene::Add(const Ref<Model>& model, const Ref<Material>& material,
        const Boozy::Transform& obTransform, const std::string& name)
    {
        SceneObject object;
        object.ID = m_NextID++;
        object.Name = name;
        object.Model = model;
        object.Material = material;
        object.ObTransform = obTransform;
        object.Visible = true;

        m_Objects.push_back(std::move(object));
        return m_Objects.back();
    }

    SceneObject* Scene::Find(uint32_t id)
    {
        for (auto& object : m_Objects)
            if (object.ID == id)
                return &object;

        return nullptr;
    }

    bool Scene::Remove(uint32_t id)
    {
        for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            if (it->ID == id)
            {
                m_Objects.erase(it);
                return true;
            }
        }
        return false;
    }

    void Scene::Clear()
    {
        m_Objects.clear();
    }
}