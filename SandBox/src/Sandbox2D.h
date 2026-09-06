#pragma once

#include "Boozy.h"

#include <glm/glm.hpp>

class Sandbox2D : public Boozy::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Boozy::Timestep delta) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Boozy::Event& e) override;
private:
    Boozy::OrthographicCameraController m_CameraController;

    Boozy::Ref<Boozy::VertexArray> m_SquareVertexArray;
    Boozy::Ref<Boozy::Shader> m_FlatColorShader;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
