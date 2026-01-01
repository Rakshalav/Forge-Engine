#pragma once

#include "../Event/Event.hpp"

namespace Forge
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event){}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

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
