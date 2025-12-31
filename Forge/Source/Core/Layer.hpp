#pragma once

#include "../Event/Event.hpp"

#include <concepts>
#include <memory>

namespace Forge
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void onEvent(Event& event){}

		virtual void onUpdate(float ts) {}
		virtual void onRender() {}

		template <std::derived_from<Layer> TLayer, typename... Args>
		inline void TransitionTo(Args&&... args)
		{
			TLayer* newLayer = new TLayer(std::forward<Args>(args)...);
			QueueTransition(newLayer, true);
		}

		template <std::derived_from<Layer> TLayer, typename... Args>
		inline void SuspendTo(Args&&... args)
		{
			TLayer* newLayer = new TLayer(std::forward<Args>(args)...);
			QueueTransition(newLayer, false);
		}

	private:
		void QueueTransition(Layer* toLayer, bool type);
	};
}
