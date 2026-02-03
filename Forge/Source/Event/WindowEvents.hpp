#pragma once

#include "Maths/Math.hpp"
#include "Event.hpp"
#include <format>

namespace fg
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) {
			m_Size.x = width; m_Size.y = height;
		}
		
		inline Vec2u GetSize() const { return m_Size; }

		std::string ToString() const override
		{
			return std::format("WindowResizeEvent: {}, {}", m_Size.x, m_Size.y);
		}

		EVENT_CLASS_TYPE(WindowResize)

	private:
		Vec2u m_Size;
	};
}