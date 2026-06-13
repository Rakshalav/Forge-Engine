#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <Forge.hpp>
	
namespace Editor
{
	class ProjectWindow
	{
	public:
		static void OnEvent(fg::Event& event);
		static void OnUpdate(float dt);
		static void OnRender();
	};
}