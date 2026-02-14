#include "Event.hpp"
#include <format>

#include "../Debug/Log.hpp"

namespace fg
{
	Event Event::KeyPressCallback(int code, int scancode, int mods, bool isRepeated)
	{
		Event event;
		event.Type = Event::Type::KeyPress;
		event.m_Data = Event::KeyPress(static_cast<Keyboard::Key>(code), scancode, static_cast<Keyboard::Mod>(mods), isRepeated);
		return event;
	}

	Event Event::KeyReleaseCallback(int code, int scancode)
	{
		Event event;
		event.Type = Event::Type::KeyRelease;
		event.m_Data = Event::KeyRelease(static_cast<Keyboard::Key>(code), scancode);
		return event;
	}

	Event Event::MouseMoveCallback(double x, double y)
	{
		Event event;
		event.Type = Event::Type::MouseMove;
		event.m_Data = Event::MouseMove({ (float)x, (float)y });
		return event;
	}

	Event Event::MouseScrollCallback(double x, double y)
	{
		Event event;
		event.Type = Event::Type::MouseScroll;
		event.m_Data = Event::MouseScroll({ (int)x, (int)y });
		return event;
	}

	Event Event::MouseButtonPressCallback(int code)
	{
		Event event;
		event.Type = Event::Type::MouseButtonPress;
		event.m_Data = Event::MouseButtonPress(static_cast<Mouse::Button>(code));
		return event;
	}

	Event Event::MouseButtonReleaseCallback(int code)
	{
		Event event;
		event.Type = Event::Type::MouseButtonRelease;
		event.m_Data = Event::MouseButtonRelease(static_cast<Mouse::Button>(code));
		return event;
	}

	Event Event::WindowResizeCallback(uint32_t x, uint32_t y)
	{
		Event event;
		event.Type = Event::Type::WindowResize;
		event.m_Data = Event::WindowResize({ x, y });
		return event;
	}

	Event Event::WindowCloseCallback()
	{
		Event event;
		event.Type = Event::Type::WindowClose;
		event.m_Data = Event::WindowClosed();
		return event;
	}

	std::string Event::ToString() const
	{
		switch (Type)
		{
		case fg::Event::Type::WindowClose:
		{
			const auto& data = std::get<WindowClosed>(m_Data);
			return "WindowCloseEvent";
		}
		case fg::Event::Type::WindowResize:
		{
			const auto& data = std::get<WindowResize>(m_Data);
			return std::format("WindowResizeEvent: {}, {}", data.Size.x, data.Size.y);
		}
		case fg::Event::Type::KeyPress:
		{
			const auto& data = std::get<KeyPress>(m_Data);
			return std::format("KeyPressedEvent: {} (repeat = {})", Keyboard::GetName(data.KeyCode), data.IsRepeated);
		}
		case fg::Event::Type::KeyRelease:
		{
			const auto& data = std::get<KeyRelease>(m_Data);
			return std::format("KeyReleasedEvent: {}", Keyboard::GetName(data.KeyCode));
		}

		case fg::Event::Type::MouseButtonPress:
		{
			const auto& data = std::get<MouseButtonPress>(m_Data);
			return std::format("MouseButtonEvent: {}", (int)data.Button);
		}
		case fg::Event::Type::MouseButtonRelease:
		{
			const auto& data = std::get<MouseButtonRelease>(m_Data);
			return std::format("MouseButtonEvent: {}", (int)data.Button);
		}
		case fg::Event::Type::MouseMove:
		{
			const auto& data = std::get<MouseMove>(m_Data);
			return std::format("MouseMovedEvent: ({}, {})", data.Offset.x, data.Offset.y);
		}
		case fg::Event::Type::MouseScroll:
		{
			const auto& data = std::get<MouseScroll>(m_Data);
			return std::format("MouseScrolledEvent: {}, {}", data.Offset.x, data.Offset.y);
		}
		default:
			return "Unknown Event";
		}
	}
}