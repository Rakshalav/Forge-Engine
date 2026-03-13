#include <glad/glad.h>
#include "OpenGLTexture.hpp"
#include <stb-image/stb_image.h>
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"

namespace fg
{
	static GLenum TextureFormatToGL(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::None:				return 0;
		case TextureFormat::R8:					return GL_R8;
		case TextureFormat::RGB8:				return GL_RGB8;
		case TextureFormat::RGBA8:				return GL_RGBA8;
		case TextureFormat::RGBA32F:			return GL_RGBA32F;
		default:								return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification) : m_Specification(specification)
	{
		GLenum internalFormat = TextureFormatToGL(specification.InternalFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, specification.Width, specification.Height);

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
			m_Specification.Width = width;
			m_Specification.Height = height;

			if (nrChannels == 1) 
			{
				m_Specification.InternalFormat = TextureFormat::R8;
				m_Specification.DataFormat = GL_RED;
			}
			else if (nrChannels == 3) //jpeg
			{
				m_Specification.InternalFormat = TextureFormat::RGB8;
				m_Specification.DataFormat = GL_RGB;
			}
			else if (nrChannels == 4) //png
			{
				m_Specification.InternalFormat = TextureFormat::RGBA8;
				m_Specification.DataFormat = GL_RGBA;
			}

			GLenum internalFormat = TextureFormatToGL(m_Specification.InternalFormat);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, m_Specification.DataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateTextureMipmap(m_RendererID);

			stbi_image_free(data);
		}
		else 
			FG_ERROR("Failed to load texture from file {}", path);
	}

	void OpenGLTexture2D::Bind() const
	{
		auto id = m_RendererID;
		RenderCommand::Submit([id]() { glBindTexture(GL_TEXTURE_2D, id); });
	}

	void OpenGLTexture2D::Activate(uint32_t slot) const
	{
		RenderCommand::Submit([slot]() { glActiveTexture(GL_TEXTURE0 + slot); });
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_Specification.DataFormat == GL_RGBA ? 4 : 3;
		if (size != m_Specification.Width * m_Specification.Height * bpp)
		{
			//TODO: add custom assertion
			FG_CRITICAL("Data must be intire texture!");
			return;
		}

		RenderCommand::Submit([this, data]() { glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, m_Specification.DataFormat, GL_UNSIGNED_BYTE, data); });
	}
}