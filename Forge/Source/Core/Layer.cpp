#include "Layer.hpp"

#include "Application.hpp"

namespace Forge
{
    void Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
    {
        auto& app = Application::Get();
        auto& layerStack = app.m_LayerStack;

        for (size_t i = 0; i < layerStack.size(); ++i)
        {
            if (layerStack[i].get() == this)
            {
                //app.m_TransitionFunctions.push_back(
                //    [&, index = i, newLayer = std::move(toLayer)]() mutable
                //    {
                //        layerStack[index] = std::move(newLayer);
                //    }
                //);
                return;
            }
        }
    }

	void Layer::QueueSuspend(std::unique_ptr<Layer> toLayer)
	{
		auto& layerStack = Application::Get().m_LayerStack;
		for (auto& layer : layerStack)
		{
			if (layer.get() == this)
			{
	
			}
		}
	}
}
