#include "Camera.hpp"

namespace Forge
{
	Camera::Camera(CameraProjection projectionType) : m_Projection(projectionType)
	{

	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_ViewDirty = true;
	}

	void Camera::SetOrientation(const glm::quat& orientation)
	{
		m_Orientation = orientation;
		m_ViewDirty = true;
	}

	void Camera::SetPerspective(float Fov, float AspectRatio, float NearClip, float FarClip)
	{
		m_Fov = Fov;
		m_AspectRatio = AspectRatio;
		m_NearClip = NearClip;
		m_FarClip = FarClip;
	}

	void Camera::SetOrthoGraphic(float Size, float AspectRatio, float NearClip, float FarClip)
	{
		m_OrthographicSize = Size;
		m_AspectRatio = AspectRatio;
		m_NearClip = NearClip;
		m_FarClip = FarClip;
	}

	void Camera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		if (height == 0)
			return;
		m_AspectRatio = (float)width / (float)height;
		m_ProjectionDirty = true;
	}

	const glm::vec3 Camera::GetFront() const
	{
		return m_Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	const glm::vec3 Camera::GetRight() const
	{
		return m_Orientation * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	const glm::vec3 Camera::GetUp() const
	{
		return m_Orientation * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		if (m_ProjectionDirty)
		{
			switch (m_Projection)
			{
				case Forge::CameraProjection::Perspective:
				{
					m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
					break;
				}

				case Forge::CameraProjection::OrthoGraphic:
				{
					float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
					float right = m_OrthographicSize * m_AspectRatio * 0.5f;
					float bottom = -m_OrthographicSize * 0.5f;
					float top = m_OrthographicSize * 0.5f;
					m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
					break;
				}

				default:
				{
					m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
					break;
				}
			}

			m_ProjectionDirty = false;
		}

		return m_ProjectionMatrix;
	}

	const glm::mat4& Camera::GetViewMatrix() const
	{
		if (m_ViewDirty)
		{
			glm::mat4 rotation = glm::mat4_cast(m_Orientation);
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
			m_ViewMatrix = glm::inverse(translation * rotation);
			m_ViewDirty = false;
		}

		return m_ViewMatrix;
	}

	const glm::mat4& Camera::GetViewProjectionMatrix() const
	{
		if (m_ViewDirty || m_ProjectionDirty)
		{
			m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
		}
		return m_ViewProjectionMatrix;
	}
}