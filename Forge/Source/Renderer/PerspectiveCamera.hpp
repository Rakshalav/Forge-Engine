#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.hpp"

namespace Forge
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera();

		//setters
		void SetPosition(const glm::vec3& position);
		void SetOrientation(const glm::quat& orientation);

		inline void SetFOV(float fov) { m_Fov = fov; }
		inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
		inline void SetViewPortSize(const glm::vec2& size) { m_ViewPortSize = size; }
		inline void SetZoomLevel(float zoomlevel) { m_ZoomLevel = zoomlevel; }

		//getters
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetFront() const { return m_Front; }
		inline const glm::vec3& GetRight() const { return m_Right; }

		inline const glm::quat& GetOrientation() const { return m_Orientation; }
		inline const float GetFOV() const { return m_Fov; }
		inline const float GetZoomLevel() const { return m_ZoomLevel; }

		const glm::mat4& GetViewMatrix() const override { return m_View; }

	private:
		void RecalculateView();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;

		glm::mat4 m_View;

		glm::quat m_Orientation{ 1.0f, 0.0f, 0.0f, 0.0f };

		float m_Fov;
		float m_AspectRatio = 1.778f;
		float m_ZoomLevel;

		glm::vec2 m_InitialMousePosition{ 0.0f, 0.0f };
		glm::vec2 m_ViewPortSize{ 1280, 720 };
	};
}