#include <GLFW/glfw3.h>

#include "Application.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Debug/Log.hpp"

#include <ranges>

namespace fg
{
	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification) 
		:	m_Specification(specification)
	{
		s_Application = this;

		glfwInit();

		Log::Init();

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Specification.WindowSpec.EventCallback = [this](Event& event) { RaiseEvent(event); };

		m_Window = CreateRef<Window>(m_Specification.WindowSpec);
		m_Window->Create();

		Renderer::Init();
		RenderCommand::ToggleDepthTesting(true);
		RenderCommand::ToggleFaceCulling(true);
	}

	Application::~Application()
	{
		Renderer::ShutDown();
		Log::UnInit();

		m_Window->Destroy();
		glfwTerminate();

		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		float lastTime = GetTime();

		for (const auto& layer : m_LayerStack.GetLayerStack())
			layer->OnAttach();

		while (m_Running)
		{
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			RenderCommand::Clear();

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

	Vec2f Application::GetFrameBufferSize() const
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
			if (command.type == 1) //transition
			{
				auto it = std::find(stack.begin(), stack.end(), command.current);
				if (it != stack.end())
				{
					delete* it; 
					*it = command.next;
				}
			}

			else if (command.type == 2) //suspend
			{
				
			}
		}

		m_Commands.clear();
	}
}