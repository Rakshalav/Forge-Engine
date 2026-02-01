#pragma once

#include "Camera.hpp"
#include "Controller.hpp"
#include "../Event/InputEvent.hpp"
#include "../Event/WindowEvents.hpp"

namespace Forge
{
	class PerspectiveCameraController : public Controller
	{
	public:
		PerspectiveCameraController(Camera* camera);

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;

		inline void SetMouseSensitivity(float newSensitivity) { m_MouseSensitivity = newSensitivity; }
		inline void SetCameraMovementSpeed(float speed) { m_CameraSpeed = speed; }
		inline const void YawShouldInvert(bool value) { m_InvertedYaw = value; }
		inline const void PitchShouldInvert(bool value) { m_InvertedPitch = value; }

		inline const float GetMouseSensitivity() const { return m_MouseSensitivity; }
		inline const bool YawInverted() const { return m_InvertedYaw; }
		inline const bool PitchInverted() const { return m_InvertedPitch; }

	private:
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnKeyBoardPressed(KeyPressedEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:
		Camera& m_Camera;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		float m_LastMouseX;
		float m_LastMouseY;
		bool  m_FirstMouse = true;
		
		float m_MouseSensitivity = 1.0f;

		float m_CameraSpeed = 5.0f;

		bool m_InvertedYaw = false;
		bool m_InvertedPitch = false;

		float m_Constain = 89.0f;

		bool m_isCursorEnabled = false;
	};
}