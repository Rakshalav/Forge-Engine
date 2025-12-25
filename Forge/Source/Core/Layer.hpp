#pragma once

#include "Event.hpp"

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

		template <std::derived_from<Layer> T, typename... Args>
		inline void TransitionTo(Args&&... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}

		template <std::derived_from<Layer> T, typename... Args>
		inline void SuspendTo(Args&&... args)
		{
			QueueSuspend(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}

	private:
		void QueueTransition(std::unique_ptr<Layer> toLayer);
		void QueueSuspend(std::unique_ptr<Layer> toLayer);
	};
}
