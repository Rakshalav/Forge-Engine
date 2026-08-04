#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <Overlay/Overlay.hpp>

namespace fg
{
	void OverlayLayer::OnEvent(Event& event)
	{
		if (m_BlockEvents)
		{
			auto& io = ImGui::GetIO();
			bool mouseEvent = event.IsInCategory(EventCategory_Mouse) && io.WantCaptureMouse;
			bool keyboardEvent = event.IsInCategory(EventCategory_Keyboard) && io.WantCaptureKeyboard;

			if (mouseEvent || keyboardEvent)
				event.Handled = true;
		}
	}

    void OverlayLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

#ifdef _WIN32
        char modulePath[MAX_PATH] = {};
        DWORD modulePathLength = GetModuleFileNameA(nullptr, modulePath, MAX_PATH);
        if (modulePathLength > 0)
            m_IniFilePath = (std::filesystem::path(modulePath).parent_path() / "imgui.ini").string();
        else
            m_IniFilePath = (std::filesystem::current_path() / "imgui.ini").string();
#else
        m_IniFilePath = (std::filesystem::current_path() / "imgui.ini").string();
#endif

        io.IniFilename = m_IniFilePath.c_str();

        //TODO: Don't make it hardcoded.
        io.Fonts->AddFontFromFileTTF("C:/Dev/Forge/tools/editor/assets/fonts/Inter.ttf", 15.0f);

		auto& style = ImGui::GetStyle();

		style.WindowRounding = 5.0f;
		style.ChildRounding = 4.0f;
		style.FrameRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.GrabRounding = 3.0f;
		style.TabRounding = 4.0f;

		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.FramePadding = ImVec2(8.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);

		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;

		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		SetDarkThemeColors();

        m_ViewportEnabled = io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
        if (m_ViewportEnabled)
            style.Colors[ImGuiCol_WindowBg].w = 1.00f;

        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

	void OverlayLayer::OnDetach()
	{
		if (m_ViewportEnabled)
			ImGui::DestroyPlatformWindows();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void OverlayLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OverlayLayer::End()
	{
		auto& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(m_Window->GetSize().x, m_Window->GetSize().y);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (m_ViewportEnabled)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void OverlayLayer::SetDarkThemeColors()
	{
		ImGui::StyleColorsDark();

		ImVec4* colors = ImGui::GetStyle().Colors;

		// Window & Panel Backgrounds
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 0.98f };

		// Title Bar
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.30f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.30f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.30f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };

		// Selection Accents 
		colors[ImGuiCol_CheckMark] = ImVec4(0.18f, 0.50f, 0.86f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18f, 0.50f, 0.86f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.18f, 0.50f, 0.86f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.18f, 0.50f, 0.86f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.18f, 0.50f, 0.86f, 0.25f);

		// Borders & Separators
		colors[ImGuiCol_Border] = ImVec4(0.26f, 0.265f, 0.27f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.205f, 0.21f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.18f, 0.50f, 0.86f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.50f, 0.86f, 1.00f);

		// Text
		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	}

	uint32_t OverlayLayer::GetActiveWidgetID()
	{
		return GImGui->ActiveId;
	}
}