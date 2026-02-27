#pragma once

#include "../Maths/Math.hpp"

#include <string>
#include <functional>

#include "../Event/Event.hpp"

struct GLFWwindow;

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

	class Application;

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
		void SetShouldClose(bool value);

		void ToggleCursor(bool value);

		float GetTime();

	private:
		void PollEvents();
		friend class Application;

	private:
		WindowSpecification m_Specification;

		GLFWwindow* m_Handle = nullptr;

		friend struct Keyboard;
		friend struct Mouse;
	};
}	