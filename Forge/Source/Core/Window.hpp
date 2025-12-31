#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <functional>

#include "../Event/Event.hpp"

namespace Forge
{
	struct WindowSpecification
	{
		std::string Title;
		uint32_t Width = 1920;
		uint32_t Height = 1080;
		bool IsResizable = true;
		bool Vsync = true;

		using EventCallbackfn = std::function<void(Event&)>;
		EventCallbackfn EventCallback;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& specification = WindowSpecification());
		~Window();

		bool Create();
		void Destroy();

		void Update();

		void RaiseEvent(Event& event);

		glm::vec2 GetFrameBufferSize() const;
		glm::vec2 GetMousePos() const;

		bool ShouldClose() const;

		inline GLFWwindow* GetHandle() const { return m_Handle; }

	private:
		WindowSpecification m_Specification;

		GLFWwindow* m_Handle = nullptr;
	};
}	