#include <Renderer/VertexArray.hpp>
#include <Renderer/RendererAPI.hpp>
#include <OpenGL/OpenGLVertexArray.hpp>

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