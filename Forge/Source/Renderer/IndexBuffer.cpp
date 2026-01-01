#include <glad/glad.h>
#include "IndexBuffer.hpp"

namespace Forge
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int size) : m_Size(size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	const unsigned int IndexBuffer::GetCount() const
	{
		return m_Size / sizeof(unsigned int);
	}
}