#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "ImGuiLayer.hpp"

namespace fg
{
	void ImGuiLayer::OnEvent(Event& event)
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

	void ImGuiLayer::OnAttach()
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

		ImGui::StyleColorsDark();

		auto& style = ImGui::GetStyle();
		m_ViewportEnabled = io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
		if (m_ViewportEnabled)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(m_Window.GetHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		if (m_ViewportEnabled)
			ImGui::DestroyPlatformWindows();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		auto& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(m_Window.GetSize().x, m_Window.GetSize().y);

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

	uint32_t ImGuiLayer::GetActiveWidgetID()
	{
		return GImGui->ActiveId;
	}
}