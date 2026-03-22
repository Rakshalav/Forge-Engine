#pragma once

#include "Renderer/Cubemap.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"

namespace fg
{
	class OpenGLCubemap final : public Cubemap
	{
	public:
		OpenGLCubemap(std::array<std::string, 6> texturePaths);

		void LoadCubemap(std::array<std::string, 6>& texturePaths) override;

		void Draw(Ref<Shader>& shader, Camera& camera) override;

	private:
		uint32_t m_RendererID;
		Ref<VertexArray> m_VertexArray;
	};
}