#pragma once

#include "../Core/Base.hpp"
#include <vector>

namespace Forge
{
	enum class LayoutType
	{
		Pos,
		PosColTex,
		PosCol,
		PosTex,
		PosNorm,
		PosNormTex
	};

	struct BufferElement
	{
		uint32_t Count;
		uint32_t Type;
		uint8_t Normalized;
		size_t Size;

		static size_t GetSizeOfType(uint32_t type);
	};

	class BufferLayout
	{
	public:
		BufferLayout() : m_Stride(0) {}

		template<LayoutType lt>
		void Push(const uint32_t& type);

		void Push(uint32_t count, uint32_t type, uint8_t normalized = false);

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const size_t GetStride() const { return m_Stride; }

	private:
		std::vector<BufferElement> m_Elements;
		size_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};

	template<LayoutType lt>
	inline void BufferLayout::Push(const uint32_t& type)
	{
		switch (lt)
		{
		case Forge::LayoutType::Pos:
			Push(3, type);
			return;
		case Forge::LayoutType::PosCol:
			Push(3, type);
			Push(3, type);
			return;
		case Forge::LayoutType::PosTex:
			Push(3, type);
			Push(2, type);
			return;
		case Forge::LayoutType::PosColTex:
			Push(3, type);
			Push(3, type);
			Push(2, type);
			return;
		}
	}
}