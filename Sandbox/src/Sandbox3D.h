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

    Boozy::Ref<Boozy::Model> m_Model;
    Boozy::Transform m_ModelTransform;

    Boozy::Ref<Boozy::Shader> m_TextureShader3D;
    Boozy::Ref<Boozy::Shader> m_FlatColorShader3D;
};