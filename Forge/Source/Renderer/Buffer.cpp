#include <glad/glad.h>

#include "Renderer.hpp"
#include "Buffer.hpp"

#include "../Platform/OpenGL/OpenGLBuffer.hpp"

namespace Forge
{
	size_t BufferElement::GetSizeOfType(uint32_t type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		default:
			return 0;
		}
	}

	void BufferLayout::Push(uint32_t count, uint32_t type, uint8_t normalized)
	{
		m_Elements.push_back({ count, type, normalized, count * BufferElement::GetSizeOfType(type) });
		m_Stride += count * BufferElement::GetSizeOfType(type);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			//TODO: add assertion
			return nullptr;
			
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);

		case RendererAPI::Vulkan:
			//TODO: add assertion vulkan api not supported!
			return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			//TODO: add assertion
			return nullptr;

		case RendererAPI::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);

		case RendererAPI::Vulkan:
			//TODO: add assertion vulkan api not supported!
			return nullptr;
		}
	}
}