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

    Boozy::Ref<Boozy::Texture2D> m_Texture;

    Boozy::Ref<Boozy::Mesh> m_CubeMesh;

    Boozy::Ref<Boozy::Shader> m_TextureShader3D;

    glm::vec3 m_CubeRotationSpeed = { 90.0f, 0.0f, 180.0f };
};