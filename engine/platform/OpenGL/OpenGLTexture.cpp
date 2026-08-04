#include <glad/glad.h>
#include "OpenGLTexture.hpp"
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"

namespace fg
{
#pragma region Utils
	static GLenum TextureInternalFormatToOGL(TextureSpecification::InternalFormat format)
	{
		switch (format)
		{
		case TextureSpecification::InternalFormat::None:				return 0;
		case TextureSpecification::InternalFormat::R8:					return GL_R8;
		case TextureSpecification::InternalFormat::RG8:					return GL_RG8;
		case TextureSpecification::InternalFormat::RGB8:				return GL_RGB8;
		case TextureSpecification::InternalFormat::RGBA8:				return GL_RGBA8;
		case TextureSpecification::InternalFormat::SRGB8:				return GL_SRGB8;
		case TextureSpecification::InternalFormat::SRGBA8:				return GL_SRGB8_ALPHA8;

		case TextureSpecification::InternalFormat::R16F:				return GL_R16F;      
		case TextureSpecification::InternalFormat::RG16F:				return GL_RG16F;
		case TextureSpecification::InternalFormat::RGB16F:				return GL_RGB16F;
		case TextureSpecification::InternalFormat::RGBA16F:				return GL_RGBA16F;

		case TextureSpecification::InternalFormat::R32F:				return GL_R32F;
		case TextureSpecification::InternalFormat::RG32F:				return GL_RG32F;     
		case TextureSpecification::InternalFormat::RGB32F:				return GL_RGB32F;
		case TextureSpecification::InternalFormat::RGBA32F:				return GL_RGBA32F;

		case TextureSpecification::InternalFormat::R32I:				return GL_R32I;
		case TextureSpecification::InternalFormat::RG32I:				return GL_RG32I;
		case TextureSpecification::InternalFormat::RGB32I:				return GL_RGB32I;
		case TextureSpecification::InternalFormat::RGBA32I:				return GL_RGBA32I;

		case TextureSpecification::InternalFormat::DEPTH24:				return GL_DEPTH_COMPONENT24;
		case TextureSpecification::InternalFormat::DEPTH24STENSIL8:		return GL_DEPTH24_STENCIL8;
		default: return 0;
		}
	}

	static GLenum TextureFormatToOGL(TextureSpecification::InternalFormat format)
	{
		switch (format)
		{
		case TextureSpecification::InternalFormat::R8:
		case TextureSpecification::InternalFormat::R16F:
		case TextureSpecification::InternalFormat::R32F:		return GL_RED;

		case TextureSpecification::InternalFormat::RG8:
		case TextureSpecification::InternalFormat::RG16F:
		case TextureSpecification::InternalFormat::RG32F:		return GL_RG;        

		case TextureSpecification::InternalFormat::RGB8:
		case TextureSpecification::InternalFormat::SRGB8:     
		case TextureSpecification::InternalFormat::RGB16F:
		case TextureSpecification::InternalFormat::RGB32F:		return GL_RGB;

		case TextureSpecification::InternalFormat::RGBA8:
		case TextureSpecification::InternalFormat::SRGBA8:    
		case TextureSpecification::InternalFormat::RGBA16F:
		case TextureSpecification::InternalFormat::RGBA32F:		return GL_RGBA;

		case TextureSpecification::InternalFormat::RG32I:		return GL_RED_INTEGER;

		case TextureSpecification::InternalFormat::DEPTH24STENSIL8: return GL_DEPTH_ATTACHMENT;
		default: return 0;
		}
	}

	static GLint TextureWrapToOGL(TextureSpecification::Wrap wrap)
	{
		switch (wrap)
		{
		case fg::TextureSpecification::Wrap::Repeat:			return GL_REPEAT;
		case fg::TextureSpecification::Wrap::ClampToEdge:		return GL_CLAMP_TO_EDGE;
		case fg::TextureSpecification::Wrap::MirroredRepeat:	return GL_MIRRORED_REPEAT;
		}
	}

	static GLint TextureFilterToOGL(TextureSpecification::Filter filter)
	{
		switch (filter)
		{
		case fg::TextureSpecification::Filter::Linear:				return GL_LINEAR;
		case fg::TextureSpecification::Filter::Nearest:				return GL_NEAREST;
		case fg::TextureSpecification::Filter::LinearMipmapLinear:	return GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	static GLenum GetGLDataType(TextureSpecification::InternalFormat format)
	{
		switch (format)
		{
		case TextureSpecification::InternalFormat::R16F:
		case TextureSpecification::InternalFormat::RG16F:
		case TextureSpecification::InternalFormat::RGB16F:
		case TextureSpecification::InternalFormat::RGBA16F:		return GL_HALF_FLOAT;
		case TextureSpecification::InternalFormat::R32F:
		case TextureSpecification::InternalFormat::RG32F:
		case TextureSpecification::InternalFormat::RGB32F:
		case TextureSpecification::InternalFormat::RGBA32F:		return GL_FLOAT;
		default: return GL_UNSIGNED_BYTE;
		}
	}

	static uint32_t GetBytesPerChannel(GLenum dataType)
	{
		switch (dataType)
		{
		case GL_FLOAT:      return 4;
		case GL_HALF_FLOAT: return 2; 
		default:            return 1; 
		}
	}

#pragma endregion

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification) : m_Specification(specification)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, TextureInternalFormatToOGL(m_Specification.Format), specification.Width, specification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOGL(m_Specification.FilterMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOGL(m_Specification.FilterMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapToOGL(m_Specification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapToOGL(m_Specification.WrapMode));
	}

	void OpenGLTexture2D::Bind() const
	{
		RenderCommand::Submit([&]() { glBindTexture(GL_TEXTURE_2D, m_RendererID); });
	}

	void OpenGLTexture2D::Activate(uint32_t slot) const
	{
		RenderCommand::Submit([slot]() { glActiveTexture(GL_TEXTURE0 + slot); });
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		GLenum format = TextureFormatToOGL(m_Specification.Format);

		GLenum dataType = GetGLDataType(m_Specification.Format);
		uint32_t bytesPerChannel = GetBytesPerChannel(dataType);
		
		uint32_t channels = (format == GL_RGBA) ? 4 : (format == GL_RGB ? 3 : 1);
		uint32_t expectedSize = m_Specification.Width * m_Specification.Height * channels * bytesPerChannel;

		if (size != expectedSize)
		{
			FG_CORE_CRITICAL("Data must be entire texture! Expected {} bytes, got {}", expectedSize, size);
			return;
		}

		RenderCommand::Submit([this, format, dataType, data]() { glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, format, dataType, data); });
	}
}