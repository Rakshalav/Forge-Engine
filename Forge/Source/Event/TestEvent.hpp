#pragma once

#include "Input.hpp"
#include "../Maths/Math.hpp"
#include <variant>
#include <functional>
#include <string>

namespace fg
{
	class Event
	{
	public:
		enum class Type
		{
			WindowClose, 
			WindowResize,
			KeyPressed, 
			KeyReleased,
			MouseButtonPressed, 
			MouseButtonReleased, 
			MouseMoved, 
			MouseScrolled,

			Count
		};

	public:
		//--- KeyBoard ---
		struct KeyPressed
		{
			Input::KeyBoard KeyCode;
			int ScanCode;
			Input::KeyMod Mods;
			bool IsRepeated;
		};

		struct KeyReleased
		{
			Input::KeyBoard KeyCode;
			int ScanCode;
		};

		//--- Mouse ---
		struct MouseMoved
		{
			Vec2f Offset;
		};

		struct MouseScrolled
		{
			Vec2i Offset;
		};

		struct MouseButtonPressed
		{
			Mouse::Button Button;
		};

		struct MouseButtonReleased
		{
			Mouse::Button Button;
		};

		//--- Window ---
		struct WindowClosed {};

		struct WindowResized
		{
			Vec2u Size;
		};

	public:
		static Event _KeyPressed(int code, int scancode, int mods, bool isRepeated);
		static Event _KeyReleased(int code, int scancode);

		static Event _MouseMoved(double x, double y);
		static Event _MouseScrolled(double x, double y);
		static Event _MouseButtonPressed(int code);
		static Event _MouseButtonReleased(int code);

		static Event _WindowResized(uint32_t x, uint32_t y);
		static Event _WindowClosed();

		template<typename T>
		inline static T& GetData() { return std::get<T>(m_Data); }

		std::string ToString() const;

	public:
		Type Type;
		bool Handled = false;

	private:
		std::variant<
			KeyPressed, 
			KeyReleased, 
			MouseMoved, 
			MouseScrolled, 
			MouseButtonPressed, 
			MouseButtonReleased,
			WindowClosed,
			WindowResized> m_Data;

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