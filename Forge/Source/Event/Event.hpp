#pragma once

#include "Input.hpp"
#include "../Maths/Math.hpp"
#include <variant>
#include <functional>
#include <string>

namespace fg
{
	class Window;

	class Event
	{
	public:
		enum class Type
		{
			WindowClose,
			WindowResize,
			KeyPress,
			KeyRelease,
			MouseButtonPress,
			MouseButtonRelease,
			MouseMove,
			MouseScroll,

			Count
		};

	public:
		//--- KeyBoard ---
		struct KeyPress
		{
			Keyboard::Key KeyCode;
			int ScanCode;
			Keyboard::Mod Mods;
			bool IsRepeated;
		};

		struct KeyRelease
		{
			Keyboard::Key KeyCode;
			int ScanCode;
		};

		//--- Mouse ---
		struct MouseMove
		{
			Vec2f Offset;
		};

		struct MouseScroll
		{
			Vec2i Offset;
		};

		struct MouseButtonPress
		{
			Mouse::Button Button;
		};

		struct MouseButtonRelease
		{
			Mouse::Button Button;
		};

		//--- Window ---
		struct WindowClose {};

		struct WindowResize
		{
			Vec2u Size;
		};

	public:
		template<typename T>
		inline static T& GetData() { return std::get<T>(m_Data); }

		std::string ToString() const;

	private:
		static Event KeyPressCallback(int code, int scancode, int mods, bool isRepeated);
		static Event KeyReleaseCallback(int code, int scancode);

		static Event MouseMoveCallback(double x, double y);
		static Event MouseScrollCallback(double x, double y);
		static Event MouseButtonPressCallback(int code);
		static Event MouseButtonReleaseCallback(int code);

		static Event WindowResizeCallback(uint32_t x, uint32_t y);
		static Event WindowCloseCallback();

		friend class Window;

	public:
		Type Type;
		bool Handled = false;

	private:
		std::variant<
			KeyPress,
			KeyRelease,
			MouseMove,
			MouseScroll,
			MouseButtonPress,
			MouseButtonRelease,
			WindowClose,
			WindowResize> m_Data;

		friend class EventDispatcher;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T>
		inline bool Dispatch(const std::function<bool(T&)>& func)
		{
			if (std::holds_alternative<T>(m_Event.m_Data) && !m_Event.Handled)
			{
				m_Event.Handled = func(std::get<T>(m_Event.m_Data));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
}