#pragma once

#include <Forge.hpp>
#include <ImGui/imgui.h>

namespace fg::Editor
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

	private:
		void OnEvent(fg::Event& event) override;
		void OnUpdate(float ts) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnAttach() override;
		void OnDetach() override;

	private:
		Ref<fg::Camera> m_Camera;
		Scope<fg::PerspectiveCameraController> m_Controller;

		Ref<fg::Shader> m_Shader;
		Scope<fg::Model> m_Model;
		
		Ref<Framebuffer> m_FrameBuffer;
	};
}