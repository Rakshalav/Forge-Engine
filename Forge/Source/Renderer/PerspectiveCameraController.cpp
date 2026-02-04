#include "PerspectiveCameraController.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Event/Input.hpp"
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

		if (Input::IsKeyPressed(KeyBoard::W)) 
		{
			position += velocity * front;
			m_Camera.SetPosition(position);
		}
		if (Input::IsKeyPressed(KeyBoard::S))
		{
			position -= velocity * front;
			m_Camera.SetPosition(position);
		}
		if (Input::IsKeyPressed(KeyBoard::A))
		{
			position -= right * velocity;
			m_Camera.SetPosition(position);
		}
		if (Input::IsKeyPressed(KeyBoard::D))
		{
			position += right * velocity;
			m_Camera.SetPosition(position);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Event::MouseMoved>([this](Event::MouseMoved& e) { return OnMouseMoved(e); });
		dispatcher.Dispatch<Event::MouseScrolled>([this](Event::MouseScrolled& e) { return OnMouseScrolled(e); });
		dispatcher.Dispatch<Event::KeyPressed>([this](Event::KeyPressed& e) { return OnKeyBoardPressed(e); });
		dispatcher.Dispatch<Event::WindowResized>([this](Event::WindowResized& e) { return OnWindowResized(e); });
	}

	bool PerspectiveCameraController::OnKeyBoardPressed(Event::KeyPressed& event)
	{
		if (event.KeyCode == KeyBoard::Escape && !event.IsRepeated)
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

	bool PerspectiveCameraController::OnWindowResized(Event::WindowResized& event)
	{
		m_Camera.SetViewPortSize(event.Size.x, event.Size.y);
		Renderer::SetViewPortSize(event.Size.x, event.Size.y);
		return false;
	}

	bool PerspectiveCameraController::OnMouseMoved(Event::MouseMoved& event)
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

	bool PerspectiveCameraController::OnMouseScrolled(Event::MouseScrolled& event)
	{		
		return false;
	}
}