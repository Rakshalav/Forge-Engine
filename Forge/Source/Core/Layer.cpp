#include "Layer.hpp"

#include "Application.hpp"

namespace Forge
{
    void Layer::QueueTransition(Layer* toLayer, bool type)
    {
        auto& app = Application::Get();
        auto& layerStack = app.m_LayerStack.GetLayerStack();

		for (auto layer : layerStack)
		{
			if (layer == this)
			{
				app.m_Commands.push_back(std::make_tuple(layer, toLayer, type));
				return;
			}
		}
    }
}
