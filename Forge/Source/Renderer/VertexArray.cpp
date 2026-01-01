#include <glad/glad.h>

#include "VertexArray.hpp"

namespace Forge
{
	size_t BufferElement::GetSizeOfType(uint32_t type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		default: return 0;
		}
	}

	void BufferLayout::Push(uint32_t count, uint32_t type, uint8_t normalized)
	{
		m_Elements.push_back({ count, type, normalized, count * BufferElement::GetSizeOfType(type) });
		m_Stride += count * BufferElement::GetSizeOfType(type);
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
	{
		Bind();
		vb.Bind();

		const auto& elements = layout.GetElements();
		size_t offset = 0;

		for (uint32_t i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, layout.GetStride(), (const void*)offset);
			offset += element.Size;
		}
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const LayoutType& type)
	{
		BufferLayout layout;

		switch (type) {
		case LayoutType::Position:
			layout.Push(3, GL_FLOAT);
			break;
		case LayoutType::Position_Color:
			layout.Push(3, GL_FLOAT);
			layout.Push(3, GL_FLOAT);
			break;
		case LayoutType::Position_Texture:
			layout.Push(3, GL_FLOAT);
			layout.Push(2, GL_FLOAT);
			break;
		case LayoutType::Position_Color_Texture:
			layout.Push(3, GL_FLOAT);
			layout.Push(3, GL_FLOAT);
			layout.Push(2, GL_FLOAT);
			break;
		}

		AddBuffer(vb, layout);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
}