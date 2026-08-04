#include <Core/Layer.hpp>
#include <Core/Application.hpp>

namespace fg
{
    void Layer::QueueTransition(Layer* toLayer, uint8_t type)
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