#include "Shader.hpp"
#include "../Platform/OpenGL/OpenGLShader.hpp"
#include "RendererAPI.hpp"

namespace fg
{
	Ref<Shader> Shader::Create(const std::string& vertexpath, const std::string& fragmentpath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLShader>(vertexpath, fragmentpath);
		}
	}
}