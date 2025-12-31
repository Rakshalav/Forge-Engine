#include "LayerStack.hpp"

namespace Forge {
	LayerStack::~LayerStack()
	{
		for (auto layer : m_LayerStack)
			delete layer;
		m_LayerStack.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_LayerStack.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.begin() + m_LayerInsertIndex, layer);
		if (it != m_LayerStack.begin() + m_LayerInsertIndex)
		{
			delete* it;
			m_LayerStack.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(m_LayerStack.begin() + m_LayerInsertIndex, m_LayerStack.end(), layer);
		if (it != m_LayerStack.end())
		{
			delete* it;
			m_LayerStack.erase(it);
		}
	}
}
