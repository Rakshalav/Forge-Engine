#pragma once

#include "RendererAPI.hpp"

namespace fg
{
	class RenderCommand
	{
	public:
		static inline void Init()
		{
			s_RenderApi->Init();
		}

		static void inline SetViewPort(const Vec2u& lowerLeft, const Vec2u& size)
		{
			s_RenderApi->SetViewPort(lowerLeft, size);
		}

		static void inline ToggleDepthTesting(bool value)
		{
			s_RenderApi->ToggleDepthTesting(value);
		}

		static void inline ToggleFaceCulling(bool value)
		{
			s_RenderApi->ToggleFaceCulling(value);
		}

		static void inline DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderApi->DrawIndexed(vertexArray);
		}

		static void inline ClearColor(const Vec4f& color)
		{
			s_RenderApi->ClearColor(color);
		}

		static void inline Clear()
		{
			s_RenderApi->Clear();
		}

	private:
		static inline Scope<RendererAPI> s_RenderApi = RendererAPI::Create();
	};
}