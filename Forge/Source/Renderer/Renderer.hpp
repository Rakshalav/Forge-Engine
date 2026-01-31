#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"

namespace Forge
{
	enum class RendererAPI
	{
		None = 0,	
		OpenGL,
		Vulkan
	};

	class Renderer
	{
	public:
		static void Draw(const Ref<VertexArray>& vertexarray, const Ref<Shader>& shader);
		static Renderer& GetInstance();

		static void Init();
		static void ShutDown();

		static void Clear();
		static void ClearColor(glm::vec4& color);
		static void ClearColor(float x, float y, float z, float w);

		inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; }
		inline static RendererAPI GetAPI() { return s_RendererAPI; }

	private:
		inline static RendererAPI s_RendererAPI;
		Renderer();
	};
}