#pragma once

#include <Forge.hpp>
#include <ImGui/imgui.h>

namespace fg::Editor
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

	private:
		void OnEvent(fg::Event& event) override;
		void OnUpdate(float ts) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnAttach() override;
		void OnDetach() override;

	private:

	};
}