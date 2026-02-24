#pragma once

#include "../Core/Base.hpp"
#include <vector>

namespace fg
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

	enum class ElementType
	{
		BYTE			= 0x1400,
		UNSIGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406
	};

	struct BufferElement
	{
		uint32_t Count;
		uint32_t Type;
		uint8_t Normalized;
		uint32_t Size;
		uint32_t Offset;

		static uint8_t GetSizeOfType(ElementType type);
	};

	class BufferLayout
	{
	public:
		BufferLayout() : m_Stride(0) {}

		template<LayoutType lt>
		void Push(ElementType Etype);

		void Push(uint32_t count, ElementType Etype, uint8_t normalized = false);

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
	inline void BufferLayout::Push(ElementType Etype)
	{
		switch (lt)
		{
		case LayoutType::Pos:
			Push(3, Etype);
			return;
		case LayoutType::PosCol:
			Push(3, Etype);
			Push(3, Etype);
			return;
		case LayoutType::PosTex:
			Push(3, Etype);
			Push(2, Etype);
			return;
		case LayoutType::PosColTex:
			Push(3, Etype);
			Push(3, Etype);
			Push(2, Etype);
			return;
		case LayoutType::PosNorm:
			Push(3, Etype);
			Push(3, Etype);
			return;
		case LayoutType::PosNormTex:
			Push(3, Etype);
			Push(3, Etype);
			Push(2, Etype);
			return;
		}
	}
}