#pragma once

#include "../Source/Renderer/Texture.hpp"
#include "../Source/Maths/Math.hpp"

namespace fg
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);

		inline Vec2u GetSize() const { return m_Size; }

		void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_RendererID;

		Vec2u m_Size;
		bool m_MipMaps = true;

		uint32_t m_InternalFormat, m_DataFormat;
	};
}