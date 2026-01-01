#pragma once

#include <vector>

#include "VertexBuffer.hpp"

namespace Forge 
{
	enum class LayoutType
	{
		Position,
		Position_Color_Texture,
		Position_Color,
		Position_Texture
	};

#define LT_P LayoutType::Position
#define LT_PCT LayoutType::Position_Color_Texture
#define LT_PC LayoutType::Position_Color
#define LT_PT LayoutType::Position_Texture

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

		void Push(uint32_t count, uint32_t type, uint8_t normalized = false);

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const size_t GetStride() const { return m_Stride; }

	private:
		std::vector<BufferElement> m_Elements;
		size_t m_Stride;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout);
		void AddBuffer(const VertexBuffer& vb, const LayoutType& type);
		void Bind() const;
		void UnBind() const;

	private:
		unsigned int m_RendererID;
	};
}
