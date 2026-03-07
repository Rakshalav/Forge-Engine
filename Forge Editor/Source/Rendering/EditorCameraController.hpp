#pragma once

#include "EditorCamera.hpp"
#include <Forge.hpp>

namespace Editor
{
	class EditorCameraController
	{
	public:
		EditorCameraController(EditorCamera& camera);

		void OnEvent(fg::Event& event);
		void OnUpdate(float ts, const fg::Vec4f& size);

	private:
		bool OnMouseMoved(fg::Event::MouseMove& event);
		void HandleMouseWrap(const fg::Vec4f& size);

	private:
		EditorCamera& m_Camera;
		fg::Vec2f m_MousePos;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		float m_MaxSpeed = 20.0f;
		float m_CurrentSpeed = 0.0f;

		bool m_CamCanMove;
		bool m_FirstMouse = true;
		float m_MouseSensitivity = 0.08f;

		float m_Constain = 89.0f;
	};
}