#include "Shader.hpp"

#include "../Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer.hpp"

namespace fg
{
	Ref<Shader> Shader::Create(const std::string& vertexpath, const std::string& fragmentpath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			return nullptr;
		case Renderer::API::OpenGL:
			return CreateRef<OpenGLShader>(vertexpath, fragmentpath);
		case Renderer::API::Vulkan:
			return nullptr;
		}
		return nullptr;
	}
}