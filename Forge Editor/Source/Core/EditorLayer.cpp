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

		m_Model = fg::CreateScope<fg::Model>("C:/Dev/Forge/Sandbox/Textures/Guitar/Guitar.obj");

		fg::FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;  
		 
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
		static ImGuiID dockspaceID = ImGui::GetID("Forge Editor");
		static auto viewport = ImGui::GetMainViewport();

		if (!ImGui::DockBuilderGetNode(dockspaceID))
		{
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);
			ImGuiID dock_id_left = 0;
			ImGuiID dock_id_main = dockspaceID;
			ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);
			ImGuiID dock_id_left_top = 0;
			ImGuiID dock_id_left_bottom = 0;
			ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.50f, &dock_id_left_top, &dock_id_left_bottom);
			ImGui::DockBuilderDockWindow("Game", dock_id_main);
			ImGui::DockBuilderDockWindow("Properties", dock_id_left_top);
			ImGui::DockBuilderDockWindow("Scene", dock_id_left_bottom);
			ImGui::DockBuilderFinish(dockspaceID);
		}

		ImGui::DockSpaceOverViewport(dockspaceID, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (!ImGui::IsWindowFocused())
			m_BlockUpdates = true;
		else
			m_BlockUpdates = false;

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		m_ViewportBounds.x = windowPos.x;
		m_ViewportBounds.y = windowPos.y;
		m_ViewportBounds.z = windowPos.x + m_ViewportSize.x;
		m_ViewportBounds.w = windowPos.y + m_ViewportSize.y;

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(uintptr_t)textureID, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();

		ImGui::Begin("Properties");
		ImGui::Text("Model properties");
		ImGui::End();

		ImGui::Begin("Scene");
		ImGui::Text("Scene Hierarchy");
		ImGui::End();
	}

	void EditorLayer::OnDetach()
	{

	}
}