#pragma once

#include "Maths/Math.hpp"
#include "Core/Base.hpp"
#include "VertexArray.hpp"

namespace fg
{
	class Application;

	class RendererAPI
	{
	public:
		enum API
		{
			None, OpenGL
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewPort(const Vec2u& lowerLeft, const Vec2u& size) = 0;
		virtual void ToggleDepthTesting(bool value) = 0;
		virtual void ToggleFaceCulling(bool value) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void ClearColor(const Vec4f& color) = 0;
		virtual void Clear() = 0;

		static API GetAPI() { return s_API; }

	private:
		static Scope<RendererAPI> Create();
		friend class RenderCommand;

	private:
		static inline API s_API = OpenGL;
	};
}