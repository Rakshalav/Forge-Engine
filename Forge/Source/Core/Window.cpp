#include <glad/glad.h>

#include "Window.hpp"
#include "../Event/WindowEvents.hpp"
#include "../Event/InputEvent.hpp"
#include "../Renderer/Renderer.hpp"

#include "../Debug/Log.hpp"

namespace Forge
{
	Window::Window(const WindowSpecification& specification) : m_Specification(specification) {}

	Window::~Window()
	{
		Destroy();
	}

	bool Window::Create()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		 

		m_Handle = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title.c_str(), nullptr, nullptr);

		if (!m_Handle)
		{
			FG_CORE_CRITICAL("Failed to create window!");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Handle);

		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			static_assert("No API!");
			return false;
		case Renderer::API::OpenGL:
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				FG_CORE_CRITICAL("Failed to initialize GLAD!");
				return false;
			}   
			#ifdef _DEBUG
				Log::InitGlDebugger();
			#endif
			break;
		}
		case Renderer::API::Vulkan:
			static_assert("Vulkan not supported!");
			return false;
		}

		glfwSwapInterval(m_Specification.Vsync ? 1 : 0);

		glfwSetWindowUserPointer(m_Handle, this);

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* handle)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));
			WindowClosedEvent event;
			window.RaiseEvent(event);
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* handle, int width, int height)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));
			WindowResizeEvent event((uint32_t)width, (uint32_t)height);
			window.RaiseEvent(event);
		});

		glfwSetKeyCallback(m_Handle, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				window.RaiseEvent(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				window.RaiseEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				window.RaiseEvent(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* handle, int button, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				window.RaiseEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				window.RaiseEvent(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* handle, double xOffset, double yOffset)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));
			MouseScrolledEvent event(xOffset, yOffset);
			window.RaiseEvent(event);
		});

		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* handle, double x, double y)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));
			MouseMovedEvent event(x, y);
			window.RaiseEvent(event);
		});

		return true;
	}

	void Window::Destroy()
	{
		if (m_Handle)
		{
			glfwDestroyWindow(m_Handle);
			glfwTerminate();
		}

		m_Handle = nullptr;
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_Handle);
	}

	void Window::RaiseEvent(Event& event)
	{
		if (m_Specification.EventCallback)
			m_Specification.EventCallback(event);
	}

	glm::vec2 Window::GetFrameBufferSize() const
	{
		int width, height;
		glfwGetFramebufferSize(m_Handle, &width, &height);
		return { width, height };
	}

	glm::vec2 Window::GetMousePos() const
	{
		double x, y;
		glfwGetCursorPos(m_Handle, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	glm::vec2 Window::GetSize() const
	{
		int width, height;
		glfwGetWindowSize(m_Handle, &width, &height);
		return { static_cast<float>(width), static_cast<float>(height) };
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Handle) != 0;
	}

	void Window::ToggleCursor(bool value)
	{
		glfwSetInputMode(m_Handle, GLFW_CURSOR, value ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
}