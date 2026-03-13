#include "EditorLayer.hpp"
#include <ImGui/imgui_internal.h>

namespace Editor
{
	EditorLayer::EditorLayer() : m_Camera(EditorCameraSpecification()), m_CamController(m_Camera)
	{

	}

	void EditorLayer::OnAttach()
	{
		m_Shader = fg::Shader::Create("C:/Dev/Forge/Sandbox/Shaders/vertex_cube.glsl", "C:/Dev/Forge/Sandbox/Shaders/Model_fragment.glsl"
		);

		FG_TRACE("Creating model");
		m_Model = fg::CreateScope<fg::Model>("C:/Dev/Forge/Sandbox/Textures/Guitar/Guitar.obj");

		fg::FramebufferSpecification spec;
		spec.Width = 1;
		spec.Height = 1;  
		 
		spec.Attachments = { fg::FramebufferTextureFormat::RGBA8, fg::FramebufferTextureFormat::DEPTH24STENCIL8 };

		m_FrameBuffer = fg::Framebuffer::Create(spec);
	}

	void EditorLayer::OnEvent(fg::Event& event)
	{
		if (!m_BlockUpdates)
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

		if (!m_BlockUpdates)
			m_CamController.OnUpdate(ts, m_ViewportBounds);
	}

	void EditorLayer::OnRender()
	{
		m_Shader->Bind();

		m_FrameBuffer->Bind();
		fg::RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		fg::RenderCommand::Clear();
		fg::Renderer::BeginScene(m_Camera);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(fg::Application::Get().GetTime() * 50), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		m_Shader->SetMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));
		m_Shader->SetMat4("u_model", model);
		m_Model->Draw(m_Shader);

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

		Console();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::Viewport()
	{
		ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			m_BlockUpdates = !ImGui::IsWindowFocused();

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_ViewportBounds = {
				windowPos.x,
				windowPos.y,
				windowPos.x + m_ViewportSize.x,
				windowPos.y + m_ViewportSize.y
			};

			uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)(uintptr_t)textureID, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}

	void EditorLayer::Console()
	{
		ImGui::Begin("Console");
		if (ImGui::Button("Clear")) fg::Log::GetClientSink()->Clear();
		ImGui::Separator();

		ImGui::BeginChild("LogRegion");
		auto& logs = fg::Log::GetClientSink()->GetMessages();

		for (const auto& log : logs)
		{
			ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; // Default White

			switch (log.Level)
			{
			case spdlog::level::warn:     color = { 1.0f, 0.8f, 0.0f, 1.0f }; break; // Yellow
			case spdlog::level::err:      color = { 1.0f, 0.2f, 0.2f, 1.0f }; break; // Red
			case spdlog::level::critical: color = { 1.0f, 0.0f, 1.0f, 1.0f }; break; // Magenta
			case spdlog::level::info:     color = { 0.4f, 0.9f, 0.4f, 1.0f }; break; // Green
			case spdlog::level::trace:    color = { 0.7f, 0.7f, 0.7f, 1.0f }; break; // Gray
			}

			ImGui::TextColored(color, log.Message.c_str());
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}
}