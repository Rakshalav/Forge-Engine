#pragma once

#include "Input.hpp"
#include "../Maths/Math.hpp"
#include <variant>
#include <functional>
#include <string>
#include <type_traits>
#include <format>
#include "Debug/Log.hpp"

namespace fg
{
	enum EventCategory : uint8_t
	{
		EventCategory_None = 0,
		EventCategory_Application	= 1 << 0,
		EventCategory_Input			= 1 << 1,
		EventCategory_Keyboard		= 1 << 2,
		EventCategory_Mouse			= 1 << 3,
		EventCategory_MouseButton	= 1 << 4
	};

	enum class EventType : uint8_t
	{
		WindowClose,
		WindowResize,
		KeyPress,
		KeyRelease,
		MouseButtonPress,
		MouseButtonRelease,
		MouseMove,
		MouseScroll,
		FileChange,

		Count
	};

	class Event
	{
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
			Vec2f Position;
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

		//--- FileChange ---
		struct FileChange
		{
			std::string Directory;
			std::string FileName;
			std::string OldFilename;
			enum Actions : uint8_t { Add = 1, Delete = 2, Modified = 3, Moved = 4 } Action;
		};
				
	public:
		template<typename T>
		inline T& GetData() { return std::get<T>(m_Data); }

		std::string ToString() const;

		template<EventType T>
		static void Print(fg::Event& event)
		{
			if (event.Type == T)
				FG_TRACE("{}", event.ToString());
		}

		inline bool IsInCategory(EventCategory category) const { return (CategoryFlags & (int)category) != 0; }

		static Event KeyPressCallback(int code, int scancode, int mods, bool isRepeated);
		static Event KeyReleaseCallback(int code, int scancode);

		static Event MouseMoveCallback(double x, double y);
		static Event MouseScrollCallback(double x, double y);
		static Event MouseButtonPressCallback(int code);
		static Event MouseButtonReleaseCallback(int code);

		static Event WindowResizeCallback(uint32_t x, uint32_t y);
		static Event WindowCloseCallback();

		static Event FileChangeCallback(int Action, const std::string& dir, const std::string& fileName, const std::string& oldFileName);

	public:
		EventType Type;
		bool Handled = false;
		int CategoryFlags;

	private:
		std::variant<
			FileChange,
			KeyPress,
			KeyRelease,
			MouseMove,
			MouseScroll,
			WindowResize,
			MouseButtonPress,
			MouseButtonRelease,
			WindowClose> m_Data;

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