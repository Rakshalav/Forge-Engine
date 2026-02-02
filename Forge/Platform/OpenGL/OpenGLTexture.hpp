#pragma once

#include "../Source/Renderer/Texture.hpp"

namespace Forge
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

	private:
		uint32_t m_RendererID;

		uint32_t m_Width, m_Height;
		bool m_MipMaps = true;

		uint32_t m_InternalFormat, m_DataFormat;
	};
}

