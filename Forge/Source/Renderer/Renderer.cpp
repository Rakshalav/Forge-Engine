#include <glad/glad.h>
#include "Renderer.hpp"

namespace Forge
{
	static Renderer* s_Instance = nullptr;

	Renderer::Renderer()
	{
		s_Instance = this;
	}

	void Renderer::Draw(const Ref<VertexArray>& vertexarray, const Ref<Shader>& shader)
	{	
		shader->Bind();
		vertexarray->Bind();
		const auto& index_count = vertexarray->GetIndexBuffer()->GetCount();

		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	}

	Renderer& Renderer::GetInstance()
	{
		return *s_Instance;
	}

	void Renderer::Init()
	{
		static Renderer instance;
	}

	void Renderer::ShutDown()
	{
		s_Instance = nullptr;
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::ClearColor(glm::vec4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void Renderer::ClearColor(float x, float y, float z, float w)
	{
		glClearColor(x, y, z, w);
	}
}