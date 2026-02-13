#pragma once

#include "Maths/Math.hpp"
#include <glm/gtc/quaternion.hpp>

namespace fg 
{
    enum class CameraProjection : uint8_t
    {
        Perspective = 0,
        OrthoGraphic
    };

    class Camera 
    {
    public:
        Camera() = default;
        Camera(CameraProjection projectionType);

        void SetPosition(const Vec3f& position);
        void SetOrientation(const glm::quat& orientation);

        void SetPerspective(float Fov, float AspectRatio, float NearClip, float FarClip);
        void SetOrthoGraphic(float Size, float AspectRatio, float NearClip, float FarClip);

        void SetViewPortSize(uint32_t width, uint32_t height);

        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const ;
        const glm::mat4& GetViewProjectionMatrix() const;

        const Vec3f GetFront() const;
        const Vec3f GetRight() const;
        const Vec3f GetUp() const;

        const Vec3f& GetPosition() const { return m_Position; }

    private:
        CameraProjection m_Projection;

        Vec3f m_Position{ 0.0f, 0.0f, 0.0f };
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