#include "RendererAPI.hpp"
#include "../Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace fg
{
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::None:
			static_assert("Render Api not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		}
	}
}