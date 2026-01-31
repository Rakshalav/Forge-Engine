#include <glad/glad.h>

#include "Renderer.hpp"
#include "Buffer.hpp"

#include "../Platform/OpenGL/OpenGLBuffer.hpp"


namespace Forge
{
	uint8_t BufferElement::GetSizeOfType(ElementType type)
	{
		switch (type)
		{
			case Forge::ElementType::BYTE:				return 1;
			case Forge::ElementType::UNSIGNED_BYTE:		return 1;
			case Forge::ElementType::SHORT:				return 2;
			case Forge::ElementType::UNSIGNED_SHORT:	return 2;
			case Forge::ElementType::INT:				return 4;
			case Forge::ElementType::UNSIGNED_INT:		return 4;
			case Forge::ElementType::FLOAT:				return 4;
		}
	}

	void BufferLayout::Push(uint32_t count, ElementType Etype, uint8_t normalized)
	{
		auto type = static_cast<uint32_t>(Etype);
		auto size = (uint8_t)(count * BufferElement::GetSizeOfType(Etype));

		m_Elements.emplace_back(count, type, normalized, size);
		m_Stride += size;
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