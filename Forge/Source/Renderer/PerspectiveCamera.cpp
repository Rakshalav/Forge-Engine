#include "PerspectiveCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Forge
{
	PerspectiveCamera::PerspectiveCamera(float Fov, float AspectRatio, float NearClip, float FarClip)
		: m_Fov(Fov), m_AspectRatio(AspectRatio), m_NearClip(NearClip), m_FarClip(FarClip),
		m_Position(0.0f, 0.0f, 0.0f) 
	{
		UpdateVectors();
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_ViewDirty = true;
	}

	void PerspectiveCamera::SetOrientation(const glm::quat& orientation)
	{
		m_Orientation = orientation;
		m_ViewDirty = true;
		UpdateVectors();
	}

	const glm::vec3& PerspectiveCamera::GetEulerAngles() const
	{
		glm::vec3 radians = glm::eulerAngles(m_Orientation);
		return glm::degrees(radians);
	}

	const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const
	{
		if (m_ProjectionDirty)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
			m_ProjectionDirty = false;
		}

		return m_ProjectionMatrix;
	}

	const glm::mat4& PerspectiveCamera::GetViewMatrix() const
	{
		if (m_ViewDirty)
		{
			m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
			m_ViewDirty = false;
		}

		return m_ViewMatrix;
	}

	const glm::mat4& PerspectiveCamera::GetViewProjectionMatrix() const
	{
		if (m_ViewDirty || m_ProjectionDirty)
		{
			m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
		}
		return m_ViewProjectionMatrix;
	}

	void PerspectiveCamera::UpdateVectors()
	{
		m_Front = m_Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
		m_Up = m_Orientation * glm::vec3(0.0f, 1.0f, 0.0f);
		m_Right = m_Orientation * glm::vec3(1.0f, 0.0f, 0.0f);		
	}
}