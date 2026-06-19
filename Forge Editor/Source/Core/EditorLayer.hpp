#pragma once

#include <Forge.hpp>
#include <ImGui/imgui.h>
#include "Rendering/EditorCamera.hpp"
#include "Rendering/EditorCameraController.hpp"
#include "GUI/ProjectWindow.hpp"

namespace Editor
{
	class EditorLayer final : public fg::Layer
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
		void Viewport();
		void Console();

	private:
		EditorCamera m_Camera;
		EditorCameraController m_CamController;
		fg::Vec2f m_ViewportSize = { 0.0f, 0.0f };
		fg::Vec4f m_ViewportBounds;

		bool isViewportFocused;

		fg::Scope<fg::Scene> m_Scene;		
		fg::Ref<fg::Framebuffer> m_FrameBuffer;

		fg::Scope<ProjectWindow> m_ProjectWindow;
	};
}