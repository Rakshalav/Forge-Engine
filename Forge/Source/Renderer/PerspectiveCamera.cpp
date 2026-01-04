#include "PerspectiveCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Forge
{
	PerspectiveCamera::PerspectiveCamera()
	{
		m_Position = { 0.0f, 0.0f, 0.0f };
		m_Right = { 1.0f, 0.0f, 0.0f };
		m_Fov = 45.0f;
		RecalculateView();
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateView();
	}

	void PerspectiveCamera::SetOrientation(const glm::quat& orientation)
	{
		m_Orientation = orientation;
		RecalculateView();
	}

	void PerspectiveCamera::RecalculateView()
	{
		m_Front = m_Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
		m_Up = m_Orientation * glm::vec3(0.0f, 1.0f, 0.0f);
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}
}