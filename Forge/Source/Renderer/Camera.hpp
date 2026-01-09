#pragma once

#include <glm/glm.hpp>

namespace Forge {
    class Camera {
    public:
        virtual const glm::mat4& GetProjectionMatrix() const = 0;
        virtual const glm::mat4& GetViewMatrix() const = 0;
        virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

    protected:
        mutable glm::mat4 m_ProjectionMatrix{ 1.0f };
        mutable glm::mat4 m_ViewMatrix{ 1.0f };
        mutable glm::mat4 m_ViewProjectionMatrix{ 1.0f };
    };
}