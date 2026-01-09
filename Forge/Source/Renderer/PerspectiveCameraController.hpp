#pragma once

#include "PerspectiveCamera.hpp"
#include "CameraController.hpp"
#include "../Event/InputEvent.hpp"

namespace Forge
{
	class PerspectiveCameraController : public CameraController
	{
	public:
		PerspectiveCameraController(PerspectiveCamera* Camera) : m_Camera(*Camera) {}

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;

		inline void SetMouseSensitivity(float newSensitivity) { m_MouseSensitivity = newSensitivity; }
		inline void SetCameraMovementSpeed(float speed) { m_CameraSpeed = speed; }

		inline const float GetMouseSensitivity() const { return m_MouseSensitivity; }

	private:
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		
	private:
		PerspectiveCamera& m_Camera;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;
		bool  m_FirstMouse = true;
		float m_MouseSensitivity = 0.5f;

		float m_CameraSpeed = 5.0f;

		bool m_InvertedYaw = false;
		bool m_InvertedPitch = false;

		float m_Constain = 89.0f;
	};
}