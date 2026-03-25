#include "Cubemap.hpp"
#include "RendererAPI.hpp"
#include "OpenGL/OpenGLCubemap.hpp"

namespace fg
{
	Ref<Cubemap> Cubemap::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None: return nullptr;
		case RendererAPI::OpenGL: return CreateRef<OpenGLCubemap>(path);
		}
	}
}