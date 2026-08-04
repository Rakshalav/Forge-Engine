#pragma once

#include <Renderer/Texture.hpp>
#include <Maths/Math.hpp>
#include <string>

namespace fg
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification);

		void Bind() const override;
		void Activate(uint32_t slot = 0) const override;

		void SetData(void* data, uint32_t size) override;

		inline Vec2u GetSize() const override { return { m_Specification.Width, m_Specification.Height }; }
		inline uint32_t GetRendererID() const override { return m_RendererID; }

		bool operator== (const Texture2D& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

		inline const TextureSpecification& GetSpecification() const override { return m_Specification; }

	private:
		uint32_t m_RendererID;
		TextureSpecification m_Specification;
	};
}