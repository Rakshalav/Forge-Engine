#include <glad/glad.h>
#include "Renderer.hpp"

namespace Forge
{
	static Renderer* s_Instance = nullptr;

	void Renderer::DepthTesting(bool value)
	{
		switch (s_RendererAPI)
		{
			case API::None: break;

			case API::OpenGL:
			{
				value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
				break;
			}

			case API::Vulkan: break;
		}
	}

	void Renderer::FaceCulling(bool value)
	{
		switch (s_RendererAPI)
		{
			case Forge::Renderer::API::None: break;

			case Forge::Renderer::API::OpenGL:
			{
				if (value)
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					glFrontFace(GL_CCW);
				}

				else glDisable(GL_CULL_FACE);

				break;
			}

			case Forge::Renderer::API::Vulkan: break;
		}
	}

	Renderer::Renderer()
	{
		s_Instance = this;
	}

	void Renderer::Draw(const Ref<VertexArray>& vertexarray, const Ref<Shader>& shader)
	{	
		switch (s_RendererAPI)
		{
			case API::None: return;

			case API::OpenGL:
			{
				shader->Bind();
				vertexarray->Bind();
				const auto& index_count = vertexarray->GetIndexBuffer()->GetCount();

				glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

				break;
			}

			case API::Vulkan: return;
		}
	}

	Renderer& Renderer::GetInstance()
	{
		return *s_Instance;
	}

	void Renderer::Init(API api)
	{
		static Renderer instance;
		s_RendererAPI = api;
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

	void Renderer::SetViewPortSize(uint32_t width, uint32_t height)
	{
		switch (s_RendererAPI)
		{
		case Forge::Renderer::API::None:
			return;
		case Forge::Renderer::API::OpenGL:
			glViewport(0, 0, width, height);
			return;
		case Forge::Renderer::API::Vulkan:
			return;
		}
	}
}