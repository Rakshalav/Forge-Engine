#pragma once

#include <GLFW/glfw3.h>
#include "../Maths/Math.hpp"

#include <string>
#include <functional>

#include "../Event/Event.hpp"

namespace fg
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

		Vec2f GetFrameBufferSize() const;
		Vec2f GetMousePos() const;
		Vec2f GetSize() const;

		bool ShouldClose() const;

		inline GLFWwindow* GetHandle() const { return m_Handle; }

		void ToggleCursor(bool value);

	private:
		WindowSpecification m_Specification;

		GLFWwindow* m_Handle = nullptr;
	};
}	