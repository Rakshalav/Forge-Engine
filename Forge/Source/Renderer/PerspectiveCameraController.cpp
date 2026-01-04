#include "GLFW/glfw3.h"
#include "PerspectiveCameraController.hpp"
#include "../Core/Application.hpp"

namespace Forge
{
	void PerspectiveCameraController::OnUpdate(float ts)
	{
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
			position -= velocity * right;
			m_Camera.SetPosition(position);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += velocity * right;
			m_Camera.SetPosition(position);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return OnMouseMoved(e); });
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScrolled(e); });
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

		offsetX *= m_MouseSensitivity;
		offsetY *= m_MouseSensitivity;

		m_Yaw += offsetX;
		m_Pitch += offsetY;

		m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

		glm::quat yaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat pitch = glm::angleAxis(glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

		m_Camera.SetOrientation(yaw * pitch);

		return true;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		float yOffset = (float)event.GetYOffset();

		float zoomLevel = m_Camera.GetZoomLevel();
			
	}
}