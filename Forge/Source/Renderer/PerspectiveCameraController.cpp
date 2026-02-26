#include "PerspectiveCameraController.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Core/Application.hpp"

namespace fg
{
	PerspectiveCameraController::PerspectiveCameraController(Camera* camera) : m_Camera(*camera)
	{
	}

	void PerspectiveCameraController::OnUpdate(float ts)
	{
		if (m_isCursorEnabled)
			return;

		float velocity = m_CameraSpeed * ts;
		glm::vec3 position = m_Camera.GetPosition();
		glm::vec3 right = m_Camera.GetRight();
		glm::vec3 front = m_Camera.GetFront();

		if (Keyboard::IsPressed(Key::W))
		{
			position += velocity * front;
			m_Camera.SetPosition(position);
		}
		if (Keyboard::IsPressed(Key::S))
		{
			position -= velocity * front;
			m_Camera.SetPosition(position);
		}
		if (Keyboard::IsPressed(Key::A))
		{
			position -= right * velocity;
			m_Camera.SetPosition(position);
		}
		if (Keyboard::IsPressed(Key::D))
		{
			position += right * velocity;
			m_Camera.SetPosition(position);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Event::MouseMove>([this](Event::MouseMove& e) { return OnMouseMoved(e); });
		dispatcher.Dispatch<Event::MouseScroll>([this](Event::MouseScroll& e) { return OnMouseScrolled(e); });
		dispatcher.Dispatch<Event::KeyPress>([this](Event::KeyPress& e) { return OnKeyBoardPressed(e); });
		dispatcher.Dispatch<Event::WindowResize>([this](Event::WindowResize& e) { return OnWindowResized(e); });
	}

	bool PerspectiveCameraController::OnKeyBoardPressed(Event::KeyPress& event)
	{
		if (event.KeyCode == Key::Escape && !event.IsRepeated)
		{
			auto& window = *Application::Get().GetWindow().get();
			if (!m_isCursorEnabled)
			{
				m_isCursorEnabled = true;
				window.ToggleCursor(m_isCursorEnabled);
				return true;
			}

			if (m_isCursorEnabled)
			{
				m_isCursorEnabled = false;
				window.ToggleCursor(m_isCursorEnabled);
				return true;
			}
		}

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(Event::WindowResize& event)
	{
		m_Camera.SetViewPortSize(event.Size.x, event.Size.y);
		Renderer::OnWindowResize({ 0, 0 }, event.Size);
		return false;
	}

	bool PerspectiveCameraController::OnMouseMoved(Event::MouseMove& event)
	{
		auto posX = (float)event.Offset.x;
		auto posY = (float)event.Offset.y;

		if (m_FirstMouse)
		{
			m_LastMouseX = posX;
			m_LastMouseY = posY;
			m_FirstMouse = false;
		}

		float offsetX = posX - m_LastMouseX;
		float offsetY = m_LastMouseY - posY;

		m_LastMouseX = posX;
		m_LastMouseY = posY;

		if (m_isCursorEnabled)
			return true;

		offsetX *= m_MouseSensitivity;
		offsetY *= m_MouseSensitivity;
		
		m_InvertedYaw ? m_Yaw += offsetX : m_Yaw -= offsetX;
		m_InvertedPitch ? m_Pitch -= offsetY : m_Pitch += offsetY;

		m_Pitch = glm::clamp(m_Pitch, -m_Constain, m_Constain);

		glm::quat yaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat pitch = glm::angleAxis(glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

		m_Camera.SetOrientation(yaw * pitch);

		return true;
	}

	bool PerspectiveCameraController::OnMouseScrolled(Event::MouseScroll& event)
	{		
		return false;
	}
}