#include <glad/glad.h>
#include "OpenGLTexture.hpp"

#include <stb-image/stb_image.h>

#include "../Source/Debug/Log.hpp"

namespace fg
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Size({width, height})
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(1);
		auto* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			m_Size = { (uint32_t)width, (uint32_t)height };

			if (nrChannels == 1) 
			{
				m_InternalFormat = GL_R8;
				m_DataFormat = GL_RED;
			}
			else if (nrChannels == 3) //jpeg
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
			}
			else if (nrChannels == 4) //png
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGBA;
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, static_cast<GLenum>(m_InternalFormat), width, height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, static_cast<GLenum>(m_DataFormat), GL_UNSIGNED_BYTE, data);
			glGenerateTextureMipmap(m_RendererID);

			stbi_image_free(data);
		}
		else 
			FG_ERROR("Failed to load texture from file {}", path);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);	
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		if (size != m_Size.x * m_Size.y * bpp)
		{
			//TODO: add custom assertion
			FG_CORE_CRITICAL("Data must be intire texture!");
			return;
		}

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Size.x, m_Size.y, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}