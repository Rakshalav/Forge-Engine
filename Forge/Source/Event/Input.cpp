#include <GLFW/glfw3.h>
#include "Input.hpp"
#include "../Source/Core/Application.hpp"

namespace fg
{
	int Keyboard::GetScanCode(Key key)
	{
		return glfwGetKeyScancode(static_cast<int>(key));
	}

	bool Keyboard::IsPressed(Key key)
	{
        auto window = Application::Get().GetWindow()->m_Handle;
		if (glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS)
			return true;
		return false;
	}

	bool Keyboard::IsRepeated(Key key)
	{
		auto window = Application::Get().GetWindow()->m_Handle;
		if (glfwGetKey(window, static_cast<int>(key)) == GLFW_REPEAT)
			return true;
		return false;

	}

	bool Keyboard::IsReleased(Key key)
	{
		auto window = Application::Get().GetWindow()->m_Handle;
		if (glfwGetKey(window, static_cast<int>(key)) == GLFW_RELEASE)
			return true;
		return false;
	}

	bool Mouse::IsButtonPressed(Mouse::Button button)
	{
		auto window = Application::Get().GetWindow()->m_Handle;
		if (glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS)
			return true;
		return false;
	}

	bool Mouse::IsButtonReleased(Mouse::Button button)
	{
		auto window = Application::Get().GetWindow()->m_Handle;
		if (glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_RELEASE)
			return true;
		return false;
	}

    const char* Keyboard::GetName(Key key)
    {
        if (const char* name = glfwGetKeyName(static_cast<int>(key), 0))
            return name;

        switch (key)
        {
        case Key::Space:        return "Space";
        case Key::Escape:       return "Escape";
        case Key::Enter:        return "Enter";
        case Key::Tab:          return "Tab";
        case Key::Backspace:    return "Backspace";

        case Key::Insert:       return "Insert";
        case Key::Delete:       return "Delete";
        case Key::Home:         return "Home";
        case Key::End:          return "End";
        case Key::PageUp:       return "PageUp";
        case Key::PageDown:     return "PageDown";

        case Key::Right:        return "Right";
        case Key::Left:         return "Left";
        case Key::Down:         return "Down";
        case Key::Up:           return "Up";

        case Key::CapsLock:     return "CapsLock";
        case Key::ScrollLock:   return "ScrollLock";
        case Key::NumLock:      return "NumLock";
        case Key::PrintScreen:  return "PrintScreen";
        case Key::Pause:        return "Pause";

        case Key::F1:           return "F1";
        case Key::F2:           return "F2";
        case Key::F3:           return "F3";
        case Key::F4:           return "F4";
        case Key::F5:           return "F5";
        case Key::F6:           return "F6";
        case Key::F7:           return "F7";
        case Key::F8:           return "F8";
        case Key::F9:           return "F9";
        case Key::F10:          return "F10";
        case Key::F11:          return "F11";
        case Key::F12:          return "F12";
        case Key::F13:          return "F13";
        case Key::F14:          return "F14";
        case Key::F15:          return "F15";
        case Key::F16:          return "F16";
        case Key::F17:          return "F17";
        case Key::F18:          return "F18";
        case Key::F19:          return "F19";
        case Key::F20:          return "F20";
        case Key::F21:          return "F21";
        case Key::F22:          return "F22";
        case Key::F23:          return "F23";
        case Key::F24:          return "F24";
        case Key::F25:          return "F25";

        case Key::KP0:         return "KP0";
        case Key::KP1:         return "KP1";
        case Key::KP2:         return "KP2";
        case Key::KP3:         return "KP3";
        case Key::KP4:         return "KP4";
        case Key::KP5:         return "KP5";
        case Key::KP6:         return "KP6";
        case Key::KP7:         return "KP7";
        case Key::KP8:         return "KP8";
        case Key::KP9:         return "KP9";
        case Key::KPDecimal:   return "KPDecimal";
        case Key::KPDivide:    return "KPDivide";
        case Key::KPMultiply:  return "KPMultiply";
        case Key::KPSubtract:  return "KPSubtract";
        case Key::KPAdd:       return "KPAdd";
        case Key::KPEnter:     return "KPEnter";
        case Key::KPEqual:     return "KPEqual";

        case Key::LeftShift:   return "LeftShift";
        case Key::LeftControl: return "LeftControl";
        case Key::LeftAlt:     return "LeftAlt";
        case Key::LeftSuper:   return "LeftSuper";
        case Key::RightShift:  return "RightShift";
        case Key::RightControl:return "RightControl";
        case Key::RightAlt:    return "RightAlt";
        case Key::RightSuper:  return "RightSuper";
        case Key::Menu:        return "Menu";

        default:
            return "Unknown";
        }
    }
}