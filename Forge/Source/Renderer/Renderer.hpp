#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"

namespace fg
{
	class Renderer
	{
	public:
		enum class API
		{
			None = 0,	
			OpenGL,
			Vulkan
		};

	public:
		static void Draw(const Ref<VertexArray>& vertexarray, const Ref<Shader>& shader);
		static Renderer& GetInstance();

		static void Init(API api);
		static void ShutDown();

		static void Clear();
		static void ClearColor(glm::vec4& color);
		static void ClearColor(float x, float y, float z, float w);

		inline static void SetAPI(API api) { s_RendererAPI = api; }
		inline static API GetAPI() { return s_RendererAPI; }

		static void DepthTesting(bool value);
		static void FaceCulling(bool value);

		static void SetViewPortSize(uint32_t width, uint32_t height);

	private:
		inline static API s_RendererAPI;
		Renderer();
	};
}