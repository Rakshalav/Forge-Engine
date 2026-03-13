#pragma once

#include <Forge.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Editor
{
	struct EditorCameraSpecification
	{
		EditorCameraSpecification() = default;
		EditorCameraSpecification(float fov, float aspectRatio, float nearClip, float farClip)
		{
			Fov = fov;
			AspectRatio = aspectRatio;
			NearClip = nearClip;
			FarClip = farClip;
		}

		float Fov = 45.0f;
		float AspectRatio = 1.77778f;
		float NearClip = 0.1f;
		float FarClip = 1000.0f;
	};

	class EditorCamera : public fg::Camera
	{
	public:
		EditorCamera(const EditorCameraSpecification& specification);

		void SetPosition(const fg::Vec3f& position);
		void SetOrientation(const glm::quat& orientation);
		void SetViewPortSize(uint32_t width, uint32_t height);

		const fg::Vec3f& GetPosition() const { return m_Position; }
		EditorCameraSpecification& GetSpecification() { return m_Specification; }
		const fg::Vec2f GetViewportSize() const { return m_ViewportSize; }


		const glm::quat& GetOrientation() const { return m_Orientation; }
		const fg::Vec3f& GetRotation() const { return glm::degrees(glm::eulerAngles(m_Orientation)); }

		const fg::Vec3f GetFront() const;
		const fg::Vec3f GetRight() const;
		const fg::Vec3f GetUp() const;

		const glm::mat4& GetProjectionMatrix() override;
		const glm::mat4& GetViewMatrix() override;
		const glm::mat4& GetViewProjectionMatrix() override;

	private:
		EditorCameraSpecification m_Specification;

		fg::Vec3f m_Position;
		glm::quat m_Orientation;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		enum Flags : uint8_t
		{
			Projection_Dirty = 1 << 0,
			View_Dirty = 1 << 1
		};
		uint8_t m_DirtyFlags;

		fg::Vec2f m_ViewportSize;
	};
}