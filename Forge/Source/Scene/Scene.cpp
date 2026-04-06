#include "Scene.hpp"
#include "Entity.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer.hpp"
#include <string>
#include <array>

namespace fg
{
	void Scene::CreateSkybox(const std::string& path)
	{
		m_Skybox = EnvironmentMap::Create(path);
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
		RenderCommand::Clear();
		Renderer::BeginScene(camera);
		//RenderSystem();

		m_Skybox->Draw(camera);
	}

	void Scene::RenderSystem()
	{
		//for (uint32_t i = 0; i < m_Registery.GetPool<MeshComponent>().Get().size(); i++)
		//{
		//	auto entityID = m_Registery.GetPool<MeshComponent>().GetEntityAt(i);

		//	if (!m_Registery.HasComponent<TransformComponent>(entityID))
		//		continue;

		//	auto& mesh = m_Registery.GetPool<MeshComponent>().Get()[i];
		//	auto& transform = m_Registery.GetComponent<TransformComponent>(entityID);

		//	auto model = transform.GetTransformation();

		//	mesh.Shader->Bind();
		//	mesh.Shader->SetMat4("u_model", model);
		//	mesh.Shader->SetMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));
		//	mesh.Model->Draw();
		//}
	}
}