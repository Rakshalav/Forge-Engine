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
		Scene();

		Entity CreateEntity();
		void OnUpdate(float ts);
		void OnRender(Camera& camera);

	private:
		void RenderSystem();

	private:
		Ref<Cubemap> m_Skybox;
		Ref<Shader> m_SkyboxShader;

		Registery m_Registery;
		friend class Entity;
	};
}