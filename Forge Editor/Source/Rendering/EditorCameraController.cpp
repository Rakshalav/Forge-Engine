#include "EditorCameraController.hpp"

namespace Editor
{
	EditorCameraController::EditorCameraController(EditorCamera& camera) : m_Camera(camera)
	{
		m_Pitch = m_Camera.GetRotation().x;
		m_Yaw = m_Camera.GetRotation().y;

		m_MousePos = fg::Mouse::GetPosition();
	}

	void EditorCameraController::OnEvent(fg::Event& event)
	{
		fg::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<fg::Event::MouseMove>([this](fg::Event::MouseMove& e) {return OnMouseMoved(e); });
	}

	void EditorCameraController::OnUpdate(float ts, const fg::Vec4f& size)
	{
		auto position = m_Camera.GetPosition();
		auto right = m_Camera.GetRight();
		auto front = m_Camera.GetFront();

		auto IsAnyKeyPressed = []()->bool { return fg::Keyboard::IsPressed(fg::Key::W) || fg::Keyboard::IsPressed(fg::Key::A) || fg::Keyboard::IsPressed(fg::Key::S) || fg::Keyboard::IsPressed(fg::Key::D); };

		float target = IsAnyKeyPressed() ? m_MaxSpeed : 0.0f;

		if (fg::Keyboard::IsPressed(fg::Key::LeftShift))
			target *= 2.0f;

		m_CurrentSpeed = std::lerp((double)m_CurrentSpeed, (double)target, (double)ts);
		float velocity = m_CurrentSpeed * ts;

		if (fg::Keyboard::IsPressed(fg::Key::W))
			position += velocity * front;

		if (fg::Keyboard::IsPressed(fg::Key::S))
			position -= velocity * front;
		if (fg::Keyboard::IsPressed(fg::Key::A))
			position -= velocity * right;

		if (fg::Keyboard::IsPressed(fg::Key::D))
			position += velocity * right;

		m_Camera.SetPosition(position);

		if (fg::Mouse::IsButtonPressed(fg::Mouse::Right))
		{
			m_CamCanMove = true;
			HandleMouseWrap(size);
		}
		else 
			m_CamCanMove = false;
	}

	bool EditorCameraController::OnMouseMoved(fg::Event::MouseMove& event)
	{
		auto current_pos = event.Position;

		float x_offset = current_pos.x - m_MousePos.x;
		float y_offset = m_MousePos.y - current_pos.y;

		m_MousePos = current_pos;

		if (m_FirstMouse)
		{
			m_MousePos = current_pos;
			m_FirstMouse = false;
		}

		if (m_CamCanMove)
		{
			m_Yaw   -= x_offset * m_MouseSensitivity;
			m_Pitch += y_offset * m_MouseSensitivity;

			m_Pitch = glm::clamp(m_Pitch, -m_Constain, m_Constain);

			glm::quat yaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat pitch = glm::angleAxis(glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

			m_Camera.SetOrientation(yaw * pitch);
			return true;
		}
		return false;
	}

	void EditorCameraController::HandleMouseWrap(const fg::Vec4f& size)
	{
		fg::Vec2f mouse_pos = fg::Mouse::GetPosition();

		bool moved = false;

		fg::Vec2f min = { size.x, size.y };
		fg::Vec2f max = { size.z, size.w };

		if (mouse_pos.x >= max.x - 1)
		{
			mouse_pos.x = min.x + 1;
			moved = true;
		}
		else if (mouse_pos.x <= min.x + 1)
		{
			mouse_pos.x = max.x - 1;
			moved = true;
		}

		if (mouse_pos.y >= max.y)
		{
			mouse_pos.y = min.y + 1;
			moved = true;
		}
		else if (mouse_pos.y <= min.y)
		{
			mouse_pos.y = max.y - 1;
			moved = true;
		}

		if (moved)
		{
			fg::Mouse::SetPosition(mouse_pos.x, mouse_pos.y);
			m_MousePos = mouse_pos;
		}
	}
}