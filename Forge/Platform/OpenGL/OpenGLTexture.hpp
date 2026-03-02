#pragma once

#include "../Source/Renderer/Texture.hpp"
#include "../Source/Maths/Math.hpp"
#include <string>

namespace fg
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::string& path);

		void Bind() const override;
		void Activate(uint32_t slot = 0) const override;

		void SetData(void* data, uint32_t size) override;

		inline Vec2u GetSize() const override { return { m_Specification.Width, m_Specification.Height }; }
		inline uint32_t GetRendererID() const override { return m_RendererID; }

		inline void SetType(TextureType type) override { m_Specification.Type = type; }
		inline TextureType GetType() const override { return m_Specification.Type; }
		inline std::string GetPath() const override { return m_Path; }

		bool operator== (const Texture2D& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		uint32_t m_RendererID;
		std::string m_Path;
		TextureSpecification m_Specification;
	};
}