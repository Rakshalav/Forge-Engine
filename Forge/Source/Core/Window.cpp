#include "Window.hpp"

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

#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);			
#endif

		m_Handle = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title.c_str(), nullptr, nullptr);

		if (!m_Handle)
		{
			//TODO: ADD LOGGER, FAILED TO CREATE WINDOW
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Handle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//TODO: Failed to initialize GLAD
			return false;
		}

		glfwSwapInterval(m_Specification.Vsync ? 1 : 0);

		glfwSetWindowUserPointer(m_Handle, this);

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* handle) 
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));
			//TODO: Window close event
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* handle, int width, int height)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));
			//TODO: WINDOW RESIZE EVENT
		});

		glfwSetKeyCallback(m_Handle, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
			case GLFW_PRESS:
			case GLFW_REPEAT:
			{
				//TODO: KEY EVENT
				break;
			}
			case GLFW_RELEASE:
			{
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
				break;
			}
			case GLFW_RELEASE:
			{
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* handle, double xOffset, double yOffset)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

		});

		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* handle, double x, double y)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

		});
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

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Handle) != 0;
	}
}