#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "../Core/Base.hpp"

#include "../Platform/OpenGL/OpenGLVertexArray.hpp"

namespace fg
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		case Renderer::API::Vulkan: return nullptr;
		}

		return nullptr;
	}
}