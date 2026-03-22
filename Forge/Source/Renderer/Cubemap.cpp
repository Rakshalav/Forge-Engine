#include "Cubemap.hpp"
#include "RendererAPI.hpp"
#include "OpenGL/OpenGLCubemap.hpp"

namespace fg
{
	Ref<Cubemap> Cubemap::Create(std::array<std::string, 6>& texturePaths)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None: return nullptr;
		case RendererAPI::OpenGL: return CreateRef<OpenGLCubemap>(texturePaths);
		}
	}
}