#include "LayerStack.hpp"
#include "Debug/Log.hpp"

namespace fg 
{
	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::Clear()
	{
		if (m_LayerStack.empty())
			return;
		for (auto layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}

		m_LayerStack.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_LayerStack.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.begin() + m_LayerInsertIndex, layer);
		if (it != m_LayerStack.begin() + m_LayerInsertIndex)
		{
			(*it)->OnDetach();
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
			(*it)->OnDetach();
			delete* it;
			m_LayerStack.erase(it);
		}
	}
}