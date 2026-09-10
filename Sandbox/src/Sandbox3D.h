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
    Boozy::PerspectiveCamera m_Camera;

    Boozy::Transform m_CubeTransform;
    Boozy::Ref<Boozy::Mesh> m_CubeMesh;
    Boozy::Ref<Boozy::Shader> m_FlatColorShader3D;

    glm::vec3 m_CubeRotationSpeed = { 90.0f, 0.0f, 180.0f };
};