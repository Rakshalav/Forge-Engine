#pragma once

#include "../Event/Event.hpp"

namespace fg
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event){}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}

		template <std::derived_from<Layer> TLayer, typename... Args>
		inline void TransitionTo(Args&&... args)
		{
			TLayer* newLayer = new TLayer(std::forward<Args>(args)...);
			QueueTransition(newLayer, 1);
		}

		template <std::derived_from<Layer> TLayer, typename... Args>
		inline void SuspendTo(Args&&... args)
		{
			TLayer* newLayer = new TLayer(std::forward<Args>(args)...);
			QueueTransition(newLayer, 2);
		}

	private:
		void QueueTransition(Layer* toLayer, uint8_t type);
	};
}
