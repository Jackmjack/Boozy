#pragma once

#include "Boozy.h"

#include <vector>
#include <glm/glm.hpp>

class Sandbox3D : public Boozy::Layer
{
public:
    Sandbox3D();
    virtual ~Sandbox3D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Boozy::Timestep delta) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Boozy::Event& event) override;
private:
    Boozy::PerspectiveCameraController m_CameraController;

    Boozy::Scene m_Scene;

    std::vector<Boozy::Light> m_Lights;
    glm::vec3 m_Ambient = { 0.1f, 0.1f, 0.1f };

    Boozy::Ref<Boozy::Material> m_Material;
    Boozy::Ref<Boozy::Model> m_Model;

    std::vector<uint32_t> m_Spinning;
    float m_Rotation = 0.0f;
    float m_RotationSpeed = 45.0f;

    Boozy::Ref<Boozy::Shader> m_FloorShader;
    Boozy::Ref<Boozy::TextureCube> m_Skybox;
    Boozy::Ref<Boozy::Shader> m_SkyboxShader;
};