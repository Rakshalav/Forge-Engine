#pragma once

#include <vector>
#include "Layer.hpp"

namespace fg
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		inline std::vector<Layer*>& GetLayerStack()  { return m_LayerStack; }

	private:
		std::vector<Layer*> m_LayerStack;
		uint32_t m_LayerInsertIndex = 0;
	};
}
