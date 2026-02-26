#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "RenderCommand.hpp"

namespace fg
{
	class Renderer
	{
	public:
		static Renderer& GetInstance();

		static void Init();
		static void BeginScene(Camera& camera);
		static void Submit(const Ref<VertexArray>& vertexarray, const Ref<Shader>& shader);
		static void OnWindowResize(const Vec2u& lowerLeft, const Vec2u& size);
		static void ShutDown();

	private:
		Renderer();

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static inline Scope<SceneData> s_SceneData = CreateScope<SceneData>();
	};
}