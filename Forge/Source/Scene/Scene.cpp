#include "Scene.hpp"
#include "Entity.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer.hpp"
#include <string>
#include <array>

namespace fg
{
	Scene::Scene()
	{
		m_SkyboxShader = Shader::Create("C:/Dev/Forge/Forge Editor/Source/Shaders/SkyboxVert.glsl", "C:/Dev/Forge/Forge Editor/Source/Shaders/SkyboxFrag.glsl");

		std::array<std::string, 6> texturePaths = {
			"C:/Dev/Forge/Forge Editor/Source/Assets/Textures/Skybox/px.png",
			"C:/Dev/Forge/Forge Editor/Source/Assets/Textures/Skybox/nx.png",
			"C:/Dev/Forge/Forge Editor/Source/Assets/Textures/Skybox/py.png",
			"C:/Dev/Forge/Forge Editor/Source/Assets/Textures/Skybox/ny.png",
			"C:/Dev/Forge/Forge Editor/Source/Assets/Textures/Skybox/pz.png",
			"C:/Dev/Forge/Forge Editor/Source/Assets/Textures/Skybox/nz.png"
		};
		m_Skybox = Cubemap::Create(texturePaths);
	}

	Entity Scene::CreateEntity()
	{
		return Entity(m_Registery.CreateEntity(), this);
	}

	void Scene::OnUpdate(float ts)
	{
		
	}

	void Scene::OnRender(Camera& camera)
	{
		RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		Renderer::BeginScene(camera);
		RenderSystem();

		m_Skybox->Draw(m_SkyboxShader, camera);
	}

	void Scene::RenderSystem()
	{
		for (uint32_t i = 0; i < m_Registery.GetPool<MeshComponent>().Get().size(); i++)
		{
			auto entityID = m_Registery.GetPool<MeshComponent>().GetEntityAt(i);

			if (!m_Registery.HasComponent<TransformComponent>(entityID))
				continue;

			auto& mesh = m_Registery.GetPool<MeshComponent>().Get()[i];
			auto& transform = m_Registery.GetComponent<TransformComponent>(entityID);

			auto model = transform.GetTransformation();

			mesh.Shader->Bind();
			mesh.Shader->SetMat4("u_model", model);
			mesh.Shader->SetMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));
			mesh.Model->Draw(mesh.Shader);
		}
	}
}