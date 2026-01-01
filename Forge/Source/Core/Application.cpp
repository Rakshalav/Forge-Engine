#include "Application.hpp"
#include "../Renderer/Renderer.hpp"

#include <ranges>

namespace Forge
{
	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification) 
		:	m_Specification(specification),
			BackGroundColor(0.f, 0.f, 0.f, 1.f)
	{
		s_Application = this;

		glfwInit();

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Specification.WindowSpec.EventCallback = [this](Event& event) {RaiseEvent(event); };

		m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
		m_Window->Create();

		//TODO: initilize opengl debugger

		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::ShutDown();

		m_Window->Destroy();
		glfwTerminate();

		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		float lastTime = GetTime();

		while (m_Running)
		{
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			Renderer::ClearColor(BackGroundColor);
			Renderer::Clear();

			float currentTime = GetTime();
			float timeStep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
			lastTime = currentTime;

			for (const auto& layer : m_LayerStack.GetLayerStack())
				layer->OnUpdate(timeStep);

			//TODO: add render thread
			for (const auto& layer : m_LayerStack.GetLayerStack())
				layer->OnRender();

			ExecuteTransitions();

			m_Window->Update();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::RaiseEvent(Event& event)
	{
		for (auto& layer : std::views::reverse(m_LayerStack.GetLayerStack()))
		{
			layer->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	glm::vec2 Application::GetFrameBufferSize() const
	{
		return m_Window->GetFrameBufferSize();
	}

	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}

	float Application::GetTime()
	{
		return (float)glfwGetTime();
	}

	void Application::ExecuteTransitions()
	{
		auto& stack = m_LayerStack.GetLayerStack();

		for (auto& command : m_Commands)
		{
			if (std::get<2>(command) == true) //transition
			{
				Layer* current = std::get<0>(command);
				Layer* next = std::get<1>(command);

				auto it = std::find(stack.begin(), stack.end(), current);
				if (it != stack.end())
				{
					delete* it; 
					*it = next;   
				}
			}

			else //suspend
			{
				
			}
		}

		m_Commands.clear();
	}
}