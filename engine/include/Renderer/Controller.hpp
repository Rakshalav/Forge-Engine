#pragma once

#include "../Event/Event.hpp"

namespace fg
{
    class Controller
    {
    public:
        virtual ~Controller() = default;

        virtual void OnUpdate(float ts) = 0;
        virtual void OnEvent(Event& e) = 0;
    };
}