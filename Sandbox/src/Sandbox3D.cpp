#include "Sandbox3D.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox3D::Sandbox3D()
    : Layer("Sandbox3D"), m_CameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
{
}

void Sandbox3D::OnAttach()
{
    Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Disabled);

    m_CameraController.SetCameraPosition({0.0f, 3.0f, 0.0f});

    Boozy::Light dlight = Boozy::Light::MakeDirectional({ -0.5f, -1.0f, -0.3f }, { 0.7f, 0.7f, 0.7f });
    Boozy::Light plight = Boozy::Light::MakePoint({ 0.0f, 5.0f, -12.0f }, { 0.0f, 1.0f, 0.0f });
    Boozy::Light slight = Boozy::Light::MakeSpot({ 0.0f, 3.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, 12.5f, 17.5f);

    m_Lights.push_back(dlight);
    m_Lights.push_back(plight);
    m_Lights.push_back(slight);

    m_FloorShader = Boozy::Shader::Create("assets/shaders/Floor.glsl");

    Boozy::Ref<Boozy::Shader> shader = Boozy::Shader::Create("assets/shaders/Lit3D.glsl");
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_Material.reset(new Boozy::Material(shader, color));
    m_Material->SetDoubleSided(true);

    m_Model.reset(new Boozy::Model("assets/models/teapot.obj"));
    m_ModelTransform = Boozy::Transform({ 0.0f, 0.0f, -12.0f }, m_Rotation, { 1.0f, 1.0f, 1.0f });
}

void Sandbox3D::OnDetach()
{
    Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Normal);
}

void Sandbox3D::OnUpdate(Boozy::Timestep delta)
{

    if (Boozy::Input::IsKeyPressed(BZ_KEY_ESCAPE))
        Boozy::Application::GetInstance().GetWindow().SetCursorMode(Boozy::WindowCursorMode::Normal);

    m_CameraController.OnUpdate(delta);

    m_Rotation = { m_Rotation.x + m_RotationSpeed * delta, m_Rotation.y, m_Rotation.z + m_RotationSpeed * 2 * delta };
    m_ModelTransform = Boozy::Transform({ 0.0f, 0.0f, -12.0f }, m_Rotation, { 1.0f, 1.0f, 1.0f });

    Boozy::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer3D::BeginScene(m_CameraController.GetCamera(), m_Lights, m_Ambient);

    if (m_Model && m_Model->IsValid())
        for (const auto& entry : m_Model->GetMeshes())
            Boozy::Renderer3D::DrawMesh(m_ModelTransform, entry.Mesh, m_Material);

    Boozy::Renderer3D::DrawFloor(m_FloorShader);

    Boozy::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
    ImGui::Begin("Light Settings");

    ImGui::SeparatorText("Environment");
    ImGui::ColorEdit3("Ambient", glm::value_ptr(m_Ambient), ImGuiColorEditFlags_Float);

    // OnAttach 固定压入 3 盏灯：[0]=Directional [1]=Point [2]=Spot
    if (m_Lights.size() < 3)
    {
        ImGui::TextDisabled("Sandbox3D expects 3 preset lights (dir / point / spot).");
        ImGui::End();
        return;
    }

    // ---- [0] 方向光 ----
    ImGui::SeparatorText("Directional Light");
    ImGui::PushID(0);
    ImGui::ColorEdit3("Color", glm::value_ptr(m_Lights[0].Color), ImGuiColorEditFlags_Float);
    ImGui::DragFloat("Intensity", &m_Lights[0].Intensity, 0.02f, 0.0f, 100.0f, "%.3f");
    ImGui::DragFloat3("Direction", glm::value_ptr(m_Lights[0].Direction), 0.01f, -1.0f, 1.0f, "%.3f");
    ImGui::PopID();

    // ---- [1] 点光 ----
    ImGui::SeparatorText("Point Light");
    ImGui::PushID(1);
    ImGui::ColorEdit3("Color", glm::value_ptr(m_Lights[1].Color), ImGuiColorEditFlags_Float);
    ImGui::DragFloat("Intensity", &m_Lights[1].Intensity, 0.02f, 0.0f, 100.0f, "%.3f");
    ImGui::DragFloat3("Position", glm::value_ptr(m_Lights[1].Position), 0.05f, -100.0f, 100.0f, "%.3f");
    ImGui::DragFloat("Constant", &m_Lights[1].Constant, 0.01f, 0.001f, 10.0f, "%.4f");
    ImGui::DragFloat("Linear", &m_Lights[1].Linear, 0.001f, 0.0f, 10.0f, "%.4f");
    ImGui::DragFloat("Quadratic", &m_Lights[1].Quadratic, 0.001f, 0.0f, 10.0f, "%.4f");
    // 实际衰减倍率
    ImGui::TextDisabled("attenuation @12 = %.4f",
        1.0f / (m_Lights[1].Constant + m_Lights[1].Linear * 12.0f + m_Lights[1].Quadratic * 144.0f));
    ImGui::PopID();

    // ---- [2] 聚光 ----
    ImGui::SeparatorText("Spot Light");
    ImGui::PushID(2);
    ImGui::ColorEdit3("Color", glm::value_ptr(m_Lights[2].Color), ImGuiColorEditFlags_Float);
    ImGui::DragFloat("Intensity", &m_Lights[2].Intensity, 0.02f, 0.0f, 100.0f, "%.3f");
    ImGui::DragFloat3("Position", glm::value_ptr(m_Lights[2].Position), 0.05f, -100.0f, 100.0f, "%.3f");
    ImGui::DragFloat3("Direction", glm::value_ptr(m_Lights[2].Direction), 0.01f, -1.0f, 1.0f, "%.3f");
    ImGui::DragFloat("Constant", &m_Lights[2].Constant, 0.01f, 0.001f, 10.0f, "%.4f");
    ImGui::DragFloat("Linear", &m_Lights[2].Linear, 0.001f, 0.0f, 10.0f, "%.4f");
    ImGui::DragFloat("Quadratic", &m_Lights[2].Quadratic, 0.001f, 0.0f, 10.0f, "%.4f");
    ImGui::TextDisabled("attenuation @12 = %.4f",
        1.0f / (m_Lights[2].Constant + m_Lights[2].Linear * 12.0f + m_Lights[2].Quadratic * 144.0f));
    ImGui::SliderFloat("InnerCutOff", &m_Lights[2].InnerCutOff, 0.0f, 89.0f, "%.2f deg");
    ImGui::SliderFloat("OuterCutOff", &m_Lights[2].OuterCutOff, 0.0f, 89.0f, "%.2f deg");
    if (m_Lights[2].InnerCutOff > m_Lights[2].OuterCutOff)   // 保证 shader 里 eps > 0
        m_Lights[2].InnerCutOff = m_Lights[2].OuterCutOff;
    ImGui::PopID();

    ImGui::End();
}

void Sandbox3D::OnEvent(Boozy::Event& event)
{
    m_CameraController.OnEvent(event);
}
