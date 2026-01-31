#include <GLFW/glfw3.h>

#include "Application.hpp"
#include "../Renderer/Renderer.hpp"

#include <ranges>

namespace Forge
{
	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification) 
		:	m_Specification(specification)
	{
		s_Application = this;

		glfwInit();

		//TODO: initilize opengl debugger
		Renderer::SetAPI(RendererAPI::OpenGL);
		Renderer::Init();

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Specification.WindowSpec.EventCallback = [this](Event& event) {RaiseEvent(event); };

		m_Window = CreateRef<Window>(m_Specification.WindowSpec);
		m_Window->Create();
	}

	Application::~Application()
	{
		for (const auto& layer : m_LayerStack.GetLayerStack())
			layer->OnDetach();

		Renderer::ShutDown();

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
			if (command.type == true) //transition
			{
				auto it = std::find(stack.begin(), stack.end(), command.current);
				if (it != stack.end())
				{
					delete* it; 
					*it = command.next;
				}
			}

			else //suspend
			{
				
			}
		}

		m_Commands.clear();
	}
}