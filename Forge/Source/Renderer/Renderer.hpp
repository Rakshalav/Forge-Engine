#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

#include "../Core/Application.hpp"

namespace Forge
{
	class Renderer
	{
	public:
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
		static Renderer& GetInstance();

	private:
		Renderer();
		static void Init();
		static void ShutDown();

		static void Clear();
		static void ClearColor(glm::vec4& color);
		static void ClearColor(float x, float y, float z, float w);

		friend class Application;
	};
}

