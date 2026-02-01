#include "PerspectiveCameraController.hpp"
#include "../Core/Application.hpp"

#include "../Renderer/Renderer.hpp"

namespace Forge
{
	PerspectiveCameraController::PerspectiveCameraController(Camera* camera) : m_Camera(*camera)
	{
		auto& window = *Application::Get().GetWindow();

		m_LastMouseX = window.GetSize().x / 2;
		m_LastMouseY = window.GetSize().y / 2;
	}

	void PerspectiveCameraController::OnUpdate(float ts)
	{
		if (m_isCursorEnabled)
			return;

		float velocity = m_CameraSpeed * ts;
		glm::vec3 position = m_Camera.GetPosition();
		glm::vec3 right = m_Camera.GetRight();
		glm::vec3 front = m_Camera.GetFront();

		auto window = Application::Get().GetWindow()->GetHandle();
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		{
			position += velocity * front;
			m_Camera.SetPosition(position);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position -= velocity * front;
			m_Camera.SetPosition(position);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position -= right * velocity;
			m_Camera.SetPosition(position);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += right * velocity;
			m_Camera.SetPosition(position);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return OnMouseMoved(e); });
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScrolled(e); });
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return OnKeyBoardPressed(e); });
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResized(e); });
	}

	bool PerspectiveCameraController::OnKeyBoardPressed(KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == GLFW_KEY_ESCAPE && !event.IsRepeat())
		{
			auto& window = *Forge::Application::Get().GetWindow().get();

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

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_Camera.SetViewPortSize(event.GetWidth(), event.GetHeight());
		Renderer::SetViewPortSize(event.GetWidth(), event.GetHeight());
		return false;
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& event)
	{
		auto posX = (float)event.GetX();
		auto posY = (float)event.GetY();

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

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{		
		return false;
	}
}