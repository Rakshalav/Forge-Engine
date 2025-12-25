#pragma once

#include "Window.hpp"
#include "Event.hpp"
#include "Layer.hpp"

#include <vector>
#include <functional>

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

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		inline void PushLayer()
		{
			m_LayerStack.push_back(std::make_unique<TLayer>());
		}

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		inline TLayer* GetLayer()
		{
			for (const auto& layer : m_LayerStack)
			{
				if (auto casted = dynamic_cast<TLayer*>(layer.get()))
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

		std::vector<std::unique_ptr<Layer>> m_LayerStack;

		std::vector<std::function<void()>> m_TransitionFunctions;

		void UpdateLayerTransitions();

		friend class Layer;
	};
}
