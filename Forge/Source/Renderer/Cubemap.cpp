#include "Cubemap.hpp"
#include "RendererAPI.hpp"
#include "OpenGL/OpenGLCubemap.hpp"

namespace fg
{
	Ref<Cubemap> Cubemap::Create(const std::string& path, const Ref<Shader>& shader1, const Ref<Shader>& shader2)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None: return nullptr;
		case RendererAPI::OpenGL: return CreateRef<OpenGLCubemap>(path, shader1, shader2);
		}
	}
}