#pragma once

#include "../Source/Renderer/Buffer.hpp"

namespace Forge
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void UnBind() const override;

		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void UnBind() const override;

		const uint32_t GetCount() const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;
	};
}