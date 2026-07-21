#include "Cubemap.hpp"
#include "RendererAPI.hpp"
#include "OpenGL/OpenGLCubemap.hpp"

namespace fg
{
	Ref<EnvironmentMap> EnvironmentMap::Create(Ref<Texture2D> texture)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None: return nullptr;
		case RendererAPI::OpenGL: return CreateRef<OpenGLEnvironmentMap>(texture);
		}
	}
}