#pragma once

#include "Components.hpp"
#include "Registery.hpp"

namespace fg
{
	class Entity;

	class Scene
	{
	public:
		Entity CreateEntity();
		void OnUpdate(float ts);
		void OnRender();

	private:
		void RenderSystem();

	private:
		Registery m_Registery;

		friend class Entity;
	};
}