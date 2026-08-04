#include <Renderer/Shader.hpp>
#include <OpenGL/OpenGLShader.hpp>
#include <Renderer/RendererAPI.hpp>

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

	Ref<Shader> Shader::Create(const char* vertexCode, const char* fragmentCode)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLShader>(vertexCode, fragmentCode);
		}
	}
}