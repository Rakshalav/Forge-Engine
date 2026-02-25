#pragma once

#include "../Source/Renderer/Texture.hpp"
#include "../Source/Maths/Math.hpp"
#include <string>

namespace fg
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);

		void Bind() const override;
		void Activate(uint32_t slot = 0) const override;

		void SetData(void* data, uint32_t size) override;

		inline Vec2u GetSize() const override { return m_Size; }
		inline uint32_t GetRendererID() const override { return m_RendererID; }

		inline void SetType(TextureType type) override { m_Type = type; }
		inline TextureType GetType() const override { return m_Type; }
		inline std::string GetPath() const override { return m_Path; }

		bool operator== (const Texture2D& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		uint32_t m_RendererID;
		TextureType m_Type;
		std::string m_Path;

		Vec2u m_Size;
		uint32_t m_InternalFormat, m_DataFormat;
	};
}