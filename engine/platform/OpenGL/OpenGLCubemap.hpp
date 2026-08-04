#pragma once

#include "Renderer/Cubemap.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Maths/Math.hpp"

namespace fg
{
	class OpenGLEnvironmentMap final : public EnvironmentMap
	{
	public:
		OpenGLEnvironmentMap(Ref<Texture2D> texture);
		void Draw(Camera& camera) override;

	private:
		void CreateCubemap(uint32_t width);
		void ConvertEquirectangularImage(Ref<Texture2D> texture, uint32_t width);
		
	private:
		uint32_t m_RendererID;
		Ref<VertexArray> m_VertexArray;
	};
}