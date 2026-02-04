#include "Event.hpp"
#include <format>

#include "../Debug/Log.hpp"

namespace fg
{
	Event Event::_KeyPressed(int code, int scancode, int mods, bool isRepeated)
	{
		Event event;
		event.Type = Event::Type::KeyPressed;
		event.m_Data = Event::KeyPressed(static_cast<Input::KeyBoard>(code), scancode, static_cast<Input::KeyMod>(mods), isRepeated);
		FG_CORE_INFO("Mods: {}", mods);
		return event;
	}

	Event Event::_KeyReleased(int code, int scancode)
	{
		Event event;
		event.Type = Event::Type::KeyReleased;
		event.m_Data = Event::KeyReleased(static_cast<Input::KeyBoard>(code), scancode);
		return event;
	}

	Event Event::_MouseMoved(double x, double y)
	{
		Event event;
		event.Type = Event::Type::MouseMoved;
		event.m_Data = Event::MouseMoved({ (float)x, (float)y });
		return event;
	}

	Event Event::_MouseScrolled(double x, double y)
	{
		Event event;
		event.Type = Event::Type::MouseScrolled;
		event.m_Data = Event::MouseScrolled({ (int)x, (int)y });
		return event;
	}

	Event Event::_MouseButtonPressed(int code)
	{
		Event event;
		event.Type = Event::Type::MouseButtonPressed;
		event.m_Data = Event::MouseButtonPressed(static_cast<Mouse::Button>(code));
		return event;
	}

	Event Event::_MouseButtonReleased(int code)
	{
		Event event;
		event.Type = Event::Type::MouseButtonReleased;
		event.m_Data = Event::MouseButtonReleased(static_cast<Mouse::Button>(code));
		return event;
	}

	Event Event::_WindowResized(uint32_t x, uint32_t y)
	{
		Event event;
		event.Type = Event::Type::WindowResize;
		event.m_Data = Event::WindowResized({ x, y });
		return event;
	}

	Event Event::_WindowClosed()
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
			const auto& data = std::get<WindowResized>(m_Data);
			return std::format("WindowResizeEvent: {}, {}", data.Size.x, data.Size.y);
		}
		case fg::Event::Type::KeyPressed:
		{
			const auto& data = std::get<KeyPressed>(m_Data);
			return std::format("KeyPressedEvent: {} (repeat = {}) mods: {}", Input::GetName(data.KeyCode), data.IsRepeated, (int)data.Mods);
		}
		case fg::Event::Type::KeyReleased:
		{
			const auto& data = std::get<KeyReleased>(m_Data);
			return std::format("KeyReleasedEvent: {}", Input::GetName(data.KeyCode));
		}

		case fg::Event::Type::MouseButtonPressed:
		{
			const auto& data = std::get<MouseButtonPressed>(m_Data);
			return std::format("MouseButtonEvent: {}", (int)data.Button);
		}
		case fg::Event::Type::MouseButtonReleased:
		{
			const auto& data = std::get<MouseButtonReleased>(m_Data);
			return std::format("MouseButtonEvent: {}", (int)data.Button);
		}
		case fg::Event::Type::MouseMoved:
		{
			const auto& data = std::get<MouseMoved>(m_Data);
			return std::format("MouseMovedEvent: ({}, {})", data.Offset.x, data.Offset.y);
		}
		case fg::Event::Type::MouseScrolled:
		{
			const auto& data = std::get<MouseScrolled>(m_Data);
			return std::format("MouseScrolledEvent: {}, {}", data.Offset.x, data.Offset.y);
		}
		default:
			return "Unknown Event";
		}
	}
}