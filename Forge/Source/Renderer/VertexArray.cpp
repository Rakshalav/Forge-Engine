#include "VertexArray.hpp"
#include "RendererAPI.hpp"
#include "../Core/Base.hpp"

#include "../Platform/OpenGL/OpenGLVertexArray.hpp"

namespace fg
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None: return nullptr;
		case RendererAPI::OpenGL: return CreateRef<OpenGLVertexArray>();
		}
	}
}