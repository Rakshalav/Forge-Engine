#include "Shader.hpp"

#include "../Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer.hpp"

namespace Forge
{
	Ref<Shader> Shader::Create(const std::string& vertexpath, const std::string& fragmentpath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLShader>(vertexpath, fragmentpath);
		case RendererAPI::Vulkan:
			return nullptr;
		}
		return nullptr;
	}
}