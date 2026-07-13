#include "EditorLayer.hpp"
#include <ImGui/imgui_internal.h>
#include <Project/Project.hpp>

namespace Editor
{
	EditorLayer::EditorLayer() : m_Camera(EditorCameraSpecification()), m_CamController(m_Camera)
	{

	}

	void EditorLayer::OnAttach()
	{
		//auto shader = fg::Renderer::GetShaderLibrary().Load("Model", FG_ROOT_DIR("Forge/Source/Renderer/Shaders/Model.vert"), FG_ROOT_DIR("Forge/Source/Renderer/Shaders/Model.frag"));
		//auto model = fg::CreateRef<fg::Model>(FG_ROOT_DIR("Sandbox/Textures/Guitar/Guitar.obj"));

		m_Scene = fg::CreateScope<fg::Scene>();
		
		//m_Guitar = m_Scene->CreateEntity();
		//m_Guitar.AddComponent<fg::TransformComponent>();
		//m_Guitar.AddComponent<fg::MeshComponent>(model, shader);

		auto project = fg::Project::Load("C:/Dev/Sandbox/Sandbox.fgproj");

		m_ProjectWindow = fg::CreateScope<ContentBrowser>();

		fg::FramebufferSpecification spec;
		spec.Width = 1;
		spec.Height = 1;  
		 
		spec.Attachments = { fg::FramebufferTextureFormat::RGBA8, fg::FramebufferTextureFormat::DEPTH24STENCIL8 };

		m_FrameBuffer = fg::Framebuffer::Create(spec);
	}

	void EditorLayer::OnEvent(fg::Event& event)
	{
		if (m_ProjectWindow)
			m_ProjectWindow->OnEvent(event);

		if (!isViewportFocused)
			m_CamController.OnEvent(event);
	}

	void EditorLayer::OnUpdate(float ts)
	{
		auto& spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != (uint32_t)m_ViewportSize.x || spec.Height != (uint32_t)m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.SetViewPortSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (!isViewportFocused)
			m_CamController.OnUpdate(ts, m_ViewportBounds);

		//m_Guitar.GetComponent<fg::TransformComponent>().Rotation.y += ts * 5.0f;
		//m_Guitar.GetComponent<fg::TransformComponent>().Translation.x += cos(fg::Application::Get().GetTime()) * 5.0f * ts;
	}

	void EditorLayer::OnRender()
	{
		m_FrameBuffer->Bind();
		m_Scene->OnRender(m_Camera);
		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		static ImGuiID dockspaceID = ImGui::GetID("ForgeEditorDockspace");
		static auto viewport = ImGui::GetMainViewport();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::DockSpaceOverViewport(dockspaceID, viewport, ImGuiDockNodeFlags_None);
		ImGui::PopStyleColor();

		if (!ImGui::DockBuilderGetNode(dockspaceID))
		{
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

			ImGuiID dock_id_left = 0;
			ImGuiID game_id = dockspaceID;
			ImGui::DockBuilderSplitNode(game_id, ImGuiDir_Left, 0.20f, &dock_id_left, &game_id);

			ImGuiID properties_id = 0;
			ImGuiID sceneHierarchy_id = 0;
			ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.50f, &properties_id, &sceneHierarchy_id);

			ImGuiID console_id = 0;
			ImGui::DockBuilderSplitNode(console_id, ImGuiDir_Down, 0.20f, &console_id, &dock_id_left);

			ImGui::DockBuilderDockWindow("Game", game_id);
			ImGui::DockBuilderDockWindow("Properties", properties_id);
			ImGui::DockBuilderDockWindow("Scene", sceneHierarchy_id);
			ImGui::DockBuilderDockWindow("Console", console_id);
			ImGui::DockBuilderFinish(dockspaceID);
		}

		Viewport();

		ImGui::Begin("Properties");
		ImGui::Text("Model properties");
		ImGui::End();

		ImGui::Begin("Scene");
		ImGui::Text("Scene Hierarchy");
		ImGui::End();

		static bool show_demo_window = true;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		m_ProjectWindow->OnRender();

		Console();
	}

	void EditorLayer::OnDetach()
	{
		
	}

	void EditorLayer::Viewport()
	{
		ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			isViewportFocused = !ImGui::IsWindowFocused();

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_ViewportBounds = {
				windowPos.x,
				windowPos.y,
				windowPos.x + viewportPanelSize.x,
				windowPos.y + viewportPanelSize.y
			};

			uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)(uintptr_t)textureID, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}

	void EditorLayer::Console()
	{
		static ImGuiTextFilter logFilter;
		auto logs = fg::Log::GetClientSink()->GetMessages();

		ImGui::Begin("Log");

		ImGui::Text("Filter:");

		ImGui::SameLine();

		ImGui::SetNextItemWidth(360.0f);
		logFilter.Draw("##Filter");

		ImGui::SameLine();

		if (ImGui::Button("Clear"))
			fg::Log::GetClientSink()->Clear();

		ImGui::SameLine();

		if (ImGui::Button("Copy"))
		{
			std::string clipboardBuffer;

			for (const auto& log : logs)
				if (logFilter.PassFilter(log.Message.c_str()))
					clipboardBuffer += log.Message;
				
			if (!clipboardBuffer.empty())
				fg::Utility::CopyToClipBoard(clipboardBuffer);
		}

		ImGui::Separator();

		ImGui::BeginChild("LogRegion");

		for (const auto& log : logs)
		{
			if (!logFilter.PassFilter(log.Message.c_str()))
				continue;

			ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			switch (log.Level)
			{
			case spdlog::level::warn:     color = { 1.0f, 0.8f, 0.2f, 1.0f }; break; 
			case spdlog::level::err:      color = { 1.0f, 0.2f, 0.2f, 1.0f }; break;
			case spdlog::level::critical: color = { 1.0f, 0.0f, 1.0f, 1.0f }; break;
			case spdlog::level::info:     color = { 0.4f, 0.9f, 0.4f, 1.0f }; break;
			}

			ImGui::TextColored(color, "%s", log.Message.c_str());
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}
}