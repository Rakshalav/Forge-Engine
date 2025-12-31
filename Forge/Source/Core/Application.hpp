#pragma once

#include "Window.hpp"
#include "../Event/Event.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"

#include <tuple>

namespace Forge
{
	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		void RaiseEvent(Event& event);

		template<typename TLayer, typename... Args>
		requires(std::is_base_of_v<Layer, TLayer>)
		inline void PushLayer(Args&&... args)
		{
			TLayer* layer = new TLayer(std::forward<Args>(args)...);
			m_LayerStack.PushLayer(layer);
		}

		template<typename TLayer, typename... Args>
		requires(std::is_base_of_v<Layer, TLayer>)
		inline void PushOverlay(Args&&... args)
		{
			TLayer* overlay = new TLayer(std::forward<Args>(args)...);
			m_LayerStack.PushOverlay(overlay);
		}

		template<std::derived_from<Layer> TLayer>
		void PopLayer()
		{
			if (auto* layer = GetLayer<TLayer>())
				m_LayerStack.PopLayer(layer);
		}


		template<std::derived_from<Layer> TLayer>
		inline void PopOverlay()
		{
			if (auto* overlay = GetLayer<TLayer>())
				m_LayerStack.PopOverlay(overlay);
		}

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		inline Layer* GetLayer()
		{
			for (const auto& layer : m_LayerStack.GetLayerStack())
			{
				if (auto casted = dynamic_cast<TLayer*>(layer))
					return casted;
			}

			return nullptr;
		}

		glm::vec2 GetFrameBufferSize() const;

		inline std::shared_ptr<Window> GetWindow() const { return m_Window; }

		static Application& Get();
		static float GetTime();

	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = false;

		LayerStack m_LayerStack;

		using LayerCommand = std::tuple<Layer*, Layer*, bool>;
		std::vector<LayerCommand> m_Commands;

		void ExecuteTransitions();

		friend class Layer;
	};
}
