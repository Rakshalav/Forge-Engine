#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.hpp"

namespace Forge
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float Fov = 45.0f, float AspectRatio = 1.778f, float NearClip = 0.1f, float FarClip = 100.0f);

		//setters
		void SetPosition(const glm::vec3& position);
		void SetOrientation(const glm::quat& orientation);

		inline void SetFOV(float fov) { m_Fov = fov; }
		inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
		inline void SetZoomLevel(float zoomlevel) { m_ZoomLevel = zoomlevel; }
		inline void SetNear(float near) { m_NearClip = near; }
		inline void SetFar(float far) { m_FarClip = far; }

		//getters
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetFront() const { return m_Front; }
		inline const glm::vec3& GetRight() const { return m_Right; }
		const glm::vec3& GetEulerAngles() const;

		inline const glm::quat& GetOrientation() const { return m_Orientation; }

		inline const float GetFOV() const { return m_Fov; }
		inline const float GetZoomLevel() const { return m_ZoomLevel; }
		inline const float GetNear() const { return m_NearClip; }
		inline const float GetFar() const { return m_FarClip; }

		const glm::mat4& GetProjectionMatrix() const override;
		const glm::mat4& GetViewMatrix() const override;
		const glm::mat4& GetViewProjectionMatrix() const override;

	private:
		void UpdateVectors();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;

		glm::quat m_Orientation{ 1.0f, 0.0f, 0.0f, 0.0f };

		float m_Fov;
		float m_AspectRatio;
		float m_ZoomLevel;
		float m_NearClip;
		float m_FarClip;

		mutable bool m_ViewDirty = true;
		mutable bool m_ProjectionDirty = true;
	};
}