#include "Sandbox3D.h"
#include <algorithm>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox3D::Sandbox3D()
    : Layer("Sandbox3D"), m_CameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
{
}

void Sandbox3D::OnAttach()
{
    auto* imguiLayer = Boozy::Application::GetInstance().GetImGuiLayer();
    imguiLayer->RegisterPanel("Light Settings", Boozy::DockSlot::Left);
    imguiLayer->RegisterPanel("Scene Objects", Boozy::DockSlot::Right);

    m_CameraController.SetCameraPosition({0.0f, 3.0f, 0.0f});

    Boozy::Light dlight = Boozy::Light::MakeDirectional({ -0.5f, -1.0f, -0.3f }, { 0.7f, 0.7f, 0.7f });
    Boozy::Light plight = Boozy::Light::MakePoint({ 0.0f, 5.0f, -12.0f }, { 0.0f, 1.0f, 0.0f });
    Boozy::Light slight = Boozy::Light::MakeSpot({ 0.0f, 10.0f, -12.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, 12.5f, 17.5f);

    m_Lights.push_back(dlight);
    m_Lights.push_back(plight);
    m_Lights.push_back(slight);

    m_FloorShader = Boozy::Shader::Create("assets/shaders/Floor.glsl");

    m_SkyboxShader = Boozy::Shader::Create("assets/shaders/Skybox.glsl");
    m_Skybox = Boozy::TextureCube::Create({
        "assets/textures/skybox/right.png", "assets/textures/skybox/left.png",
        "assets/textures/skybox/top.png",   "assets/textures/skybox/bottom.png",
        "assets/textures/skybox/front.png", "assets/textures/skybox/back.png",
        });

    Boozy::Ref<Boozy::Shader> shader = Boozy::Shader::Create("assets/shaders/Lit3D.glsl");
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_Material.reset(new Boozy::Material(shader, color));
    m_Material->SetDoubleSided(true);

    m_Model.reset(new Boozy::Model("assets/models/teapot.obj"));

    const int kGridN = 3;
    const float kSpacing = 6.0f;
    const float kBaseZ = -24.0f;
    const float kOffset = (kGridN - 1) * 0.5f;

    for (int ix = 0; ix < kGridN; ++ix)
    {
        for (int iz = 0; iz < kGridN; ++iz)
        {
            glm::vec3 position = {
                (ix - kOffset) * kSpacing,
                0.0f,
                kBaseZ + (iz - kOffset) * kSpacing
            };

            std::string name = "Teapot_" + std::to_string(ix) + "_" + std::to_string(iz);

            Boozy::Ref<Boozy::Material> material(new Boozy::Material(shader, color));
            material->SetDoubleSided(true);

            const Boozy::SceneObject& object = m_Scene.Add(
                m_Model, material,
                Boozy::Transform(position, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }),
                name);

            if (ix == 1 && iz == 1)
                m_Spinning.push_back(object.ID);
        }
    }
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(Boozy::Timestep delta)
{
    m_CameraController.OnUpdate(delta);

    m_Rotation += m_RotationSpeed * delta;
    if (m_Rotation > 360.0f)
        m_Rotation -= 360.0f;

    for (uint32_t id : m_Spinning)
    {
        if (Boozy::SceneObject* object = m_Scene.Find(id))
            object->ObTransform.SetRotation({ 0.0f, m_Rotation, 0.0f });
    }

    const Boozy::Frustum cameraFrustum(m_CameraController.GetCamera().GetViewProjectionMatrix());

    Boozy::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    Boozy::RenderCommand::Clear();

    Boozy::Renderer3D::BeginScene(m_CameraController.GetCamera(), m_Lights, m_Ambient);

    Boozy::Renderer3D::DrawSkybox(m_Skybox, m_SkyboxShader);
    Boozy::Renderer3D::DrawFloor(m_FloorShader);

    Boozy::Renderer3D::BeginShadow();
    for (const Boozy::SceneObject& object : m_Scene.GetObjects())
    {
        if (!object.Visible || !object.Model || !object.Model->IsValid())
            continue;

        for (const auto& entry : object.Model->GetMeshes())
            Boozy::Renderer3D::DrawShadow(object.ObTransform, entry.Mesh);
    }
    Boozy::Renderer3D::EndShadow();

    for (const Boozy::SceneObject& object : m_Scene.GetObjects())
    {
        if (!object.Visible || !object.Model || !object.Model->IsValid())
            continue;

        for (const auto& entry : object.Model->GetMeshes())
        {
            const glm::vec3 scale = object.ObTransform.GetScale();
            const float maxScale = glm::max(scale.x, glm::max(scale.y, scale.z));
            const glm::vec3 center = glm::vec3(
                object.ObTransform.GetTransformMatrix() * glm::vec4(entry.BoundsCenter, 1.0f));

            if (!cameraFrustum.IsInside(center, entry.BoundsRadius * maxScale))
                continue;

            Boozy::Renderer3D::DrawMesh(object.ObTransform, entry.Mesh, object.Material);
        }
    }

    Boozy::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
    ImGui::SetNextWindowSize(ImVec2(360.0f, 420.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(60.0f, 60.0f), ImGuiCond_FirstUseEver);
    // ==================== 场景物体 ====================
    ImGui::Begin("Scene Objects");

    ImGui::Text("Objects: %d", (int)m_Scene.GetObjects().size());
    ImGui::SameLine();
    if (ImGui::Button("Add Teapot"))
    {
        // 复制一份材质，避免新物体和旧物体共用颜色
        Boozy::Ref<Boozy::Material> material(new Boozy::Material(
            m_Material->GetShader(), m_Material->GetColor()));
        material->SetDoubleSided(true);

        m_Scene.Add(m_Model, material,
            Boozy::Transform({ 0.0f, 0.0f, -12.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }),
            "Teapot_" + std::to_string(m_Scene.GetObjects().size()));
    }
    ImGui::Separator();

    // 延迟删除：遍历中不能改 vector
    uint32_t pendingRemove = 0;

    for (Boozy::SceneObject& object : m_Scene.GetObjects())
    {
        ImGui::PushID((int)object.ID);

        if (ImGui::CollapsingHeader(object.Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Visible", &object.Visible);

            glm::vec3 position = object.ObTransform.GetPosition();
            if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f, -200.0f, 200.0f, "%.2f"))
                object.ObTransform.SetPosition(position);

            glm::vec3 rotation = object.ObTransform.GetRotation();
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 1.0f, -360.0f, 360.0f, "%.1f"))
                object.ObTransform.SetRotation(rotation);

            glm::vec3 scale = object.ObTransform.GetScale();
            if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f, 0.01f, 100.0f, "%.2f"))
                object.ObTransform.SetScale(scale);

            if (object.Material)
            {
                glm::vec4 color = object.Material->GetColor();
                if (ImGui::ColorEdit4("Color", glm::value_ptr(color), ImGuiColorEditFlags_Float))
                    object.Material->SetColor(color);
            }

            if (ImGui::Button("Remove"))
                pendingRemove = object.ID;
        }

        ImGui::PopID();
    }

    if (pendingRemove != 0)
    {
        m_Spinning.erase(
            std::remove(m_Spinning.begin(), m_Spinning.end(), pendingRemove),
            m_Spinning.end());
        m_Scene.Remove(pendingRemove);
    }

    ImGui::End();

    // ==================== 光照 ====================
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
