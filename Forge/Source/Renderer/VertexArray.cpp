#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "../Core/Base.hpp"

#include "../Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Forge
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		case RendererAPI::Vulkan:
			return nullptr;
		}

		return nullptr;
	}
}