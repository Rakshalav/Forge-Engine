#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Forge 
{
    enum class CameraProjection
    {
        Perspective = 0,
        OrthoGraphic
    };

    class Camera 
    {
    public:
        Camera() = default;
        Camera(CameraProjection projectionType);

        void SetPosition(const glm::vec3& position);
        void SetOrientation(const glm::quat& orientation);

        void SetPerspective(float Fov, float AspectRatio, float NearClip, float FarClip);
        void SetOrthoGraphic(float Size, float AspectRatio, float NearClip, float FarClip);

        void SetViewPortSize(uint32_t width, uint32_t height);

        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const ;
        const glm::mat4& GetViewProjectionMatrix() const;

        const glm::vec3 GetFront() const;
        const glm::vec3 GetRight() const;
        const glm::vec3 GetUp() const;

        const glm::vec3& GetPosition() const { return m_Position; }

    private:
        CameraProjection m_Projection;

        glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
        glm::quat m_Orientation{ 1.0f, 0.0f, 0.0f, 0.0f };

        float m_Fov = 45.0f;
        float m_AspectRatio = 1.778f;
        float m_NearClip = 0.1f;
        float m_FarClip = 1000.0f;
        float m_OrthographicSize = 10.0f; 

        mutable glm::mat4 m_ProjectionMatrix{ 1.0f };
        mutable glm::mat4 m_ViewMatrix{ 1.0f };
        mutable glm::mat4 m_ViewProjectionMatrix{ 1.0f };

        mutable bool m_ViewDirty = true;
        mutable bool m_ProjectionDirty = true;
    };
}