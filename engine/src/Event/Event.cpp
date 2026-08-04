#include <Event/Event.hpp>
#include <Debug/Log.hpp>

namespace fg
{
	Event Event::KeyPressCallback(int code, int scancode, int mods, bool isRepeated)
	{
		Event event;
		event.Type = EventType::KeyPress;
		event.m_Data = Event::KeyPress(static_cast<Key>(code), scancode, static_cast<Keyboard::Mod>(mods), isRepeated);
		event.CategoryFlags = EventCategory_Keyboard | EventCategory_Input;
		return event;
	}

	Event Event::KeyReleaseCallback(int code, int scancode)
	{
		Event event;
		event.Type = EventType::KeyRelease;
		event.m_Data = Event::KeyRelease(static_cast<Key>(code), scancode);
		event.CategoryFlags = EventCategory_Keyboard | EventCategory_Input;
		return event;
	}

	Event Event::MouseMoveCallback(double x, double y)
	{
		Event event;
		event.Type = EventType::MouseMove;
		event.m_Data = Event::MouseMove({ (float)x, (float)y });
		event.CategoryFlags = EventCategory_Mouse | EventCategory_Input;
		return event;
	}

	Event Event::MouseScrollCallback(double x, double y)
	{
		Event event;
		event.Type = EventType::MouseScroll;
		event.m_Data = Event::MouseScroll({ (int)x, (int)y });
		event.CategoryFlags = EventCategory_Mouse | EventCategory_Input;
		return event;
	}

	Event Event::MouseButtonPressCallback(int code)
	{
		Event event;
		event.Type = EventType::MouseButtonPress;
		event.m_Data = Event::MouseButtonPress(static_cast<Mouse::Button>(code));
		event.CategoryFlags = EventCategory_Mouse | EventCategory_MouseButton | EventCategory_Input;
		return event;
	}

	Event Event::MouseButtonReleaseCallback(int code)
	{
		Event event;
		event.Type = EventType::MouseButtonRelease;
		event.m_Data = Event::MouseButtonRelease(static_cast<Mouse::Button>(code));
		event.CategoryFlags = EventCategory_Mouse | EventCategory_MouseButton | EventCategory_Input;
		return event;
	}

	Event Event::WindowResizeCallback(uint32_t x, uint32_t y)
	{
		Event event;
		event.Type = EventType::WindowResize;
		event.m_Data = Event::WindowResize({ x, y });
		event.CategoryFlags = EventCategory_Application;
		return event;
	}

	Event Event::WindowCloseCallback()
	{
		Event event;
		event.Type = EventType::WindowClose;
		event.m_Data = Event::WindowClose();
		event.CategoryFlags = EventCategory_Application;
		return event;
	}

	Event Event::FileChangeCallback(int Action, const std::string& dir, const std::string& fileName, const std::string& oldFileName)
	{
		Event event;
		event.Type = EventType::FileChange;
		event.m_Data = Event::FileChange(dir, fileName, oldFileName, static_cast<Event::FileChange::Actions>(Action));
		event.CategoryFlags = EventCategory_None;
		return event;
	}

	std::string Event::ToString() const
	{
		const char* buttons[3] = { "Left", "Right", "Middle" };
		switch (Type)
		{
		case fg::EventType::WindowClose:
		{
			const auto& data = std::get<WindowClose>(m_Data);
			return "WindowCloseEvent";
		}
		case fg::EventType::WindowResize:
		{
			const auto& data = std::get<WindowResize>(m_Data);
			return std::format("WindowResizeEvent: {}, {}", data.Size.x, data.Size.y);
		}
		case fg::EventType::KeyPress:
		{
			const auto& data = std::get<KeyPress>(m_Data);
			return std::format("KeyPressedEvent: {} (repeat = {})", Keyboard::GetName(data.KeyCode), data.IsRepeated);
		}
		case fg::EventType::KeyRelease:
		{
			const auto& data = std::get<KeyRelease>(m_Data);
			return std::format("KeyReleasedEvent: {}", Keyboard::GetName(data.KeyCode));
		}
		case fg::EventType::MouseButtonPress:
		{
			const auto& data = std::get<MouseButtonPress>(m_Data);
			return std::format("MouseButtonEvent: {}", buttons[data.Button]);
		}
		case fg::EventType::MouseButtonRelease:
		{
			const auto& data = std::get<MouseButtonRelease>(m_Data);
			return std::format("MouseButtonEvent: {}", buttons[data.Button]);
		}
		case fg::EventType::MouseMove:
		{
			const auto& data = std::get<MouseMove>(m_Data);
			return std::format("MouseMovedEvent: ({}, {})", data.Position.x, data.Position.y);
		}
		case fg::EventType::MouseScroll:
		{
			const auto& data = std::get<MouseScroll>(m_Data);
			return std::format("MouseScrolledEvent: {}, {}", data.Offset.x, data.Offset.y);
		}
		case fg::EventType::FileChange:
		{
			const auto& data = std::get<FileChange>(m_Data);
			const char* actions[4] = { "Add", "Delete", "Modified", "Moved" };
			return std::format("FileChangeEvent: Action: {}, Directory: {}, Name: {}, OldName: {}", actions[data.Action - 1], data.Directory, data.FileName, data.OldFilename);
		}
		default:
			return "Unknown Event";
		}
	}
}