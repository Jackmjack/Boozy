#pragma once

#include "Boozy.h"

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

    Boozy::DirectionalLight m_Light;

    Boozy::Ref<Boozy::Material> m_Material;

    Boozy::Ref<Boozy::Model> m_Model;
    Boozy::Transform m_ModelTransform;

    glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    float m_RotationSpeed = 45.0f;
};