#pragma once

#include "Event.hpp"

#include <glm/glm.hpp>
#include <format>

namespace Forge
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
		using vec2int = glm::vec<2, uint32_t, glm::qualifier::defaultp>;

	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_Size(width, height) {}
		
		inline vec2int GetSize() const { return m_Size; }
		inline uint32_t GetWidth() const { return m_Size.x; }
		inline uint32_t GetHeight() const { return m_Size.y; }

		std::string ToString() const override
		{
			return std::format("WindowResizeEvent: {}, {}", m_Size.x, m_Size.y);
		}

		EVENT_CLASS_TYPE(WindowResize)

	private:
		vec2int m_Size;
	};
}