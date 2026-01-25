#include "Layer.hpp"

#include "Application.hpp"

namespace Forge
{
    void Layer::QueueTransition(Layer* toLayer, bool type)
    {
        auto& app = Application::Get();
        auto& layerStack = app.m_LayerStack.GetLayerStack();

		for (auto& layer : layerStack)
		{
			if (layer == this)
			{
				Application::LayerCommand command;
				command.current = this;
				command.next = toLayer;
				command.type = type;

				app.m_Commands.push_back(command);
				return;
			}
		}
    }
}
