#include "Input.hpp"
#include "../Source/Core/Application.hpp"

namespace fg
{
	int Input::GetScanCode(KeyBoard key)
	{
		return glfwGetKeyScancode(static_cast<int>(key));
	}

	const char* Input::GetName(KeyBoard key)
	{
		auto name = glfwGetKeyName(static_cast<int>(key), 0);
		return name ? name : "Unknown";
	}

	bool Input::IsKeyPressed(KeyBoard key)
	{
		auto window = Application::Get().GetWindow()->GetHandle();
		if (glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS)
			return true;
		return false;
	}

	bool Input::IsKeyRepeated(KeyBoard key)
	{
		auto window = Application::Get().GetWindow()->GetHandle();
		if (glfwGetKey(window, static_cast<int>(key)) == GLFW_REPEAT)
			return true;
		return false;

	}

	bool Input::IsKeyReleased(KeyBoard key)
	{
		auto window = Application::Get().GetWindow()->GetHandle();
		if (glfwGetKey(window, static_cast<int>(key)) == GLFW_RELEASE)
			return true;
		return false;
	}

	bool Input::IsMouseButtonPressed(Mouse::Button button)
	{
		auto window = Application::Get().GetWindow()->GetHandle();
		if (glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS)
			return true;
		return false;
	}

	bool Input::IsMouseButtonReleased(Mouse::Button button)
	{
		auto window = Application::Get().GetWindow()->GetHandle();
		if (glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_RELEASE)
			return true;
		return false;
	}
}