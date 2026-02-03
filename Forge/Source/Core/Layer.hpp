#pragma once

#include "../Event/TestEvent.hpp"

namespace fg
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event){}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

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
