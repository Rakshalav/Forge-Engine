#pragma once

#include "Components.hpp"
#include "Registery.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Cubemap.hpp"

namespace fg
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;

		Entity CreateEntity();
		void OnUpdate(float ts);
		void OnRender(Camera& camera);

	private:
		void RenderSystem();

	private:
		Ref<EnvironmentMap> m_Skybox;

		Registery m_Registery;
		friend class Entity;
	};
}