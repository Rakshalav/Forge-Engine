#include "EditorCamera.hpp"

namespace Editor
{
	EditorCamera::EditorCamera(const EditorCameraSpecification& specification)
		:	m_Specification(specification),
			m_Position({ 0.0f, 0.0f, 0.0f }),
			m_Orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), 
			m_ProjectionMatrix(1.0f),
			m_ViewMatrix(1.0f),
			m_ViewProjectionMatrix(1.0f),
			m_DirtyFlags(0)
	{}

	void EditorCamera::SetPosition(const fg::Vec3f& position)
	{
		m_Position = position;
		m_DirtyFlags |= View_Dirty;
	}

	void EditorCamera::SetOrientation(const glm::quat& orientation)
	{
		m_Orientation = orientation;
		m_DirtyFlags |= View_Dirty;
	}

	void EditorCamera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		if (height == 0)
			return;
		m_Specification.AspectRatio = (float)width / (float)height;
		m_DirtyFlags |= Projection_Dirty;
	}

	const fg::Vec3f EditorCamera::GetFront() const
	{
		return m_Orientation * fg::Vec3f(0.0f, 0.0f, -1.0f);
	}

	const fg::Vec3f EditorCamera::GetRight() const
	{
		return m_Orientation * fg::Vec3f(1.0f, 0.0f, 0.0f);
	}

	const fg::Vec3f EditorCamera::GetUp() const
	{
		return m_Orientation * fg::Vec3f(0.0f, 1.0f, 0.0f);
	}

	const glm::mat4& EditorCamera::GetProjectionMatrix() 
	{
		if (m_DirtyFlags & Projection_Dirty)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Specification.Fov), m_Specification.AspectRatio, m_Specification.NearClip, m_Specification.FarClip);
			m_DirtyFlags &= ~Projection_Dirty;
		}

		return m_ProjectionMatrix;
	}

	const glm::mat4& EditorCamera::GetViewMatrix() 
	{
		if (m_DirtyFlags & View_Dirty)
		{
			glm::mat4 rotation = glm::mat4_cast(m_Orientation);
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
			m_ViewMatrix = glm::inverse(translation * rotation);
			m_DirtyFlags &= ~View_Dirty;
		}
		return m_ViewMatrix;
	}

	const glm::mat4& EditorCamera::GetViewProjectionMatrix() 
	{

		if (m_DirtyFlags & (View_Dirty | Projection_Dirty))
			m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
		return m_ViewProjectionMatrix;
	}
}