#include "bzpch.h"
#include "Boozy/ImGui/ImGuiLayer.h"
#include "Boozy/Renderer/GraphicsContext.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Boozy/Core/Application.h"

namespace Boozy {
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }

    ImGuiLayer::~ImGuiLayer()
    {

    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::GetInstance();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender()
    {

    }

    void ImGuiLayer::RegisterPanel(const std::string& name, DockSlot defaultSlot)
    {
        m_Panels.emplace_back(name, defaultSlot);
    }

    void ImGuiLayer::BeginDockSpace()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBackground;

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("##BoozyDockSpace", nullptr, flags);
        ImGui::PopStyleVar(3);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Reset layout: delete imgui.ini and restart", nullptr, false, false);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        const ImGuiID dockspaceID = ImGui::GetID("BoozyDockSpace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        if (!m_LayoutBuilt && viewport->WorkSize.x > 1.0f && viewport->WorkSize.y > 1.0f)
        {
            BuildDefaultLayout((unsigned int)dockspaceID);
            m_LayoutBuilt = true;
        }
    }

    void ImGuiLayer::EndDockSpace()
    {
        ImGui::End();
    }

    void ImGuiLayer::BuildDefaultLayout(unsigned int dockspaceID)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->WorkSize);

        ImGuiID centerID = dockspaceID;
        ImGuiID leftID = 0;
        ImGuiID rightID = 0;

        ImGui::DockBuilderSplitNode(centerID, ImGuiDir_Left, 0.22f, &leftID, &centerID);

        ImGuiID tempRight = 0;
        ImGui::DockBuilderSplitNode(centerID, ImGuiDir_Right, 0.28f, &rightID, &tempRight);
        centerID = tempRight;

        ImGuiID bottomID = 0;
        ImGui::DockBuilderSplitNode(rightID, ImGuiDir_Down, 0.35f, &bottomID, &rightID);

        for (const auto& [name, slot] : m_Panels)
        {
            ImGuiID target = leftID;
            switch (slot)
            {
            case DockSlot::Left:  target = leftID;   break;
            case DockSlot::Right: target = rightID;  break;
            case DockSlot::Down:  target = bottomID; break;
            }
            ImGui::DockBuilderDockWindow(name.c_str(), target);
        }

        ImGui::DockBuilderFinish(dockspaceID);
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetInstance();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GraphicsContext* context = Application::GetInstance().GetWindow().GetContext();

            void* backup_current_context = context->GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            context->MakeCurrentContext(backup_current_context);
        }
    }
}
