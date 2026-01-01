#pragma once

namespace Forge
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int size);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		const unsigned int GetCount() const;

	private:
		unsigned int m_RendererID;
		unsigned int m_Size;
	};
}

