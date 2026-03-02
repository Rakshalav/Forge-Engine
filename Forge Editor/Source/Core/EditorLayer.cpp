#include "EditorLayer.hpp"

namespace fg::Editor
{
	EditorLayer::EditorLayer()
	{
		m_Camera = fg::CreateRef<fg::Camera>(fg::CameraProjection::Perspective);
		m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

		m_Controller = fg::CreateScope<fg::PerspectiveCameraController>(m_Camera.get());
		m_Controller->SetMouseSensitivity(0.08f);
	}

	void EditorLayer::OnAttach()
	{
		m_Shader = fg::Shader::Create("C:/Dev/Forge/Sandbox/Shaders/vertex_cube.glsl", "C:/Dev/Forge/Sandbox/Shaders/Model_fragment.glsl"
		);

		m_Model = fg::CreateScope<fg::Model>("C:/Dev/Forge/Sandbox/Textures/Guitar/Guitar.obj");

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;  
		 
		spec.Attachments = { fg::FramebufferTextureFormat::RGBA8, fg::FramebufferTextureFormat::DEPTH24STENCIL8 };

		m_FrameBuffer = fg::Framebuffer::Create(spec);
	}

	void EditorLayer::OnEvent(fg::Event& event)
	{

	}

	void EditorLayer::OnUpdate(float ts)
	{

	}

	void EditorLayer::OnRender()
	{
		m_Shader->Bind();

		m_FrameBuffer->Bind();
		fg::Renderer::BeginScene(*m_Camera.get());

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
		m_Shader->SetMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));
		m_Shader->SetMat4("u_model", model);
		m_Model->Draw(m_Shader);

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::Begin("Viewport");
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f));
		ImGui::End();
	}

	void EditorLayer::OnDetach()
	{

	}
}