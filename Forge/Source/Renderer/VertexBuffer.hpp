#pragma once

namespace Forge
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);

		void Bind() const;
		void UnBind() const;

		~VertexBuffer();

	private:
		unsigned int m_RendererID;
	};
}