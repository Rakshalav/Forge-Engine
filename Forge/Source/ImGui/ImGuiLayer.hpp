#pragma once

#include "Core/Layer.hpp"
#include "Core/Window.hpp"

namespace fg
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(Window& window) : m_Window(window), m_BlockEvents(true) {}

	public:
		void OnEvent(Event& event) override;

		void OnAttach() override;
		void OnDetach() override;
		
		void Begin();
		void End();

		void BlockEvents(bool value) { m_BlockEvents = value; }
		uint32_t GetActiveWidgetID();

	private:
		Window m_Window;
		bool m_BlockEvents;
		bool m_ViewportEnabled;
	};
}