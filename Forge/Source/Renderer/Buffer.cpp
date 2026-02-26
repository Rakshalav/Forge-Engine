#include "RendererAPI.hpp"
#include "Buffer.hpp"

#include "../Platform/OpenGL/OpenGLBuffer.hpp"


namespace fg
{
	uint8_t BufferElement::GetSizeOfType(ElementType type)
	{
		switch (type)
		{
			case fg::ElementType::BYTE:				return 1;
			case fg::ElementType::UNSIGNED_BYTE:	return 1;
			case fg::ElementType::SHORT:			return 2;
			case fg::ElementType::UNSIGNED_SHORT:	return 2;
			case fg::ElementType::INT:				return 4;
			case fg::ElementType::UNSIGNED_INT:		return 4;
			case fg::ElementType::FLOAT:			return 4;
		}
	}

	void BufferLayout::Push(uint32_t count, ElementType Etype, uint8_t normalized)
	{
		uint32_t type = static_cast<uint32_t>(Etype);
		uint32_t size = (count * BufferElement::GetSizeOfType(Etype));

		m_Elements.emplace_back(count, type, normalized, size, m_Stride);
		m_Stride += size;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None:
			//TODO: add assertion
			return nullptr;
			
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None:
			//TODO: add assertion
			return nullptr;

		case RendererAPI::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		}
	}
}