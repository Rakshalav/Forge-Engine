#include <glad/glad.h>
#include "OpenGLTexture.hpp"

#include <stb-image/stb_image.h>

#include "../Source/Debug/Log.hpp"

namespace fg
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, nrChannels;
		auto* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			m_Size.x = (uint32_t)width;
			m_Size.y = (uint32_t)height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (nrChannels == 4) //is png
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGBA;
			}
			else if (nrChannels == 3)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = (uint32_t)internalFormat;
			m_DataFormat = (uint32_t)dataFormat;

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

			if (m_MipMaps) glGenerateMipmap(GL_TEXTURE_2D);
		}
		else FG_ERROR("Failed to load texture from file {}", path);

		stbi_image_free(data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}
}