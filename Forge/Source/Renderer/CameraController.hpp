#pragma once

#include "../Event/Event.hpp"

namespace Forge
{
    class CameraController
    {
    public:
        virtual ~CameraController() = default;

        virtual void OnUpdate(float ts) = 0;
        virtual void OnEvent(Event& e) = 0;
    };
}