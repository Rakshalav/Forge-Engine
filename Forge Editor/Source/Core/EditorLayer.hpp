#pragma once

#include <Forge.hpp>
#include <ImGui/imgui.h>
#include "Rendering/EditorCamera.hpp"
#include "Rendering/EditorCameraController.hpp"

namespace Editor
{
	class EditorLayer : public fg::Layer
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
		void Game_Panel();
		void UI_Console();

	private:
		EditorCamera m_Camera;
		EditorCameraController m_CamController;
		fg::Vec2f m_ViewportSize = { 0.0f, 0.0f };
		fg::Vec4f m_ViewportBounds;

		bool m_BlockUpdates;

		fg::Ref<fg::Shader> m_Shader;
		fg::Scope<fg::Model> m_Model;
		
		fg::Ref<fg::Framebuffer> m_FrameBuffer;
	};
}