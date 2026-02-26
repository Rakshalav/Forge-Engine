#include "Renderer.hpp"

namespace fg
{
	static Renderer* s_Instance = nullptr;

	Renderer::Renderer()
	{
		s_Instance = this;
		RenderCommand::Init();
	}

	Renderer& Renderer::GetInstance()
	{
		return *s_Instance;
	}

	void Renderer::Init()
	{
		static Renderer instance;
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexarray, const Ref<Shader>& shader)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(vertexarray);
	}

	void Renderer::OnWindowResize(const Vec2u& lowerLeft, const Vec2u& size)
	{
		RenderCommand::SetViewPort(lowerLeft, size);
	}

	void Renderer::ShutDown()
	{
		s_Instance = nullptr;
	}
}