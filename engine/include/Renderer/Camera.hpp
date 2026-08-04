#pragma once

#include "Maths/Math.hpp"

namespace fg 
{
    class Camera 
    {
    public:
        ~Camera() = default;

        virtual const glm::mat4& GetProjectionMatrix() = 0;
        virtual const glm::mat4& GetViewMatrix() = 0;
        virtual const glm::mat4& GetViewProjectionMatrix() = 0;  
    };
}