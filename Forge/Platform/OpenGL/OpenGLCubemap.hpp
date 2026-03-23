#pragma once

#include "Renderer/Cubemap.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Maths/Math.hpp"

namespace fg
{
	class OpenGLCubemap final : public Cubemap
	{
	public:
		OpenGLCubemap(const std::string& path, const Ref<Shader>& shader1, const Ref<Shader>& shader2);
		void Draw(Camera& camera) override;

	private:
		void LoadFile(const char* path);
		void CreateCubemap();
		void ConvertEquirectangularImage();
		
	private:
		uint32_t m_RendererID;
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_CubemapShader;
		Ref<Shader> m_ConversionShader;

		uint32_t m_TextureID;
		uint32_t m_CubemapWidth;
	};
}