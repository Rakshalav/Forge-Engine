#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "RenderCommand.hpp"
#include "Core/TaskManager.hpp"
#include "ShaderLibrary.hpp"

namespace fg
{
	class Renderer
	{
	public:
		static Renderer& GetInstance();

		static void Init();
		static void BeginScene(Camera& camera);
		static void Submit(const Ref<VertexArray>& vertexarray);
		static void SubmitRenderTask(Task* task);
		static void WaitForRenderTasks();
		static void OnWindowResize(const Vec2u& lowerLeft, const Vec2u& size);
		static void ShutDown();

		static ShaderLibrary& GetShaderLibrary() { return *s_ShaderLibrary; }

	private:
		Renderer();

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static inline SceneData* s_SceneData = new SceneData();
		static inline TaskManager* s_RenderTaskManager = &TaskManager::Get();
		static inline ShaderLibrary* s_ShaderLibrary = new ShaderLibrary();
	};
}