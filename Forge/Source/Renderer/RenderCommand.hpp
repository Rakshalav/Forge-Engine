#pragma once

#include "RendererAPI.hpp"
#include <vector>
#include <functional>

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
			Submit([lowerLeft, size]() { s_RenderApi->SetViewPort(lowerLeft, size); });
		}

		static void inline ToggleDepthTesting(bool value)
		{
			Submit([value]() { s_RenderApi->ToggleDepthTesting(value); });
		}

		static void inline ToggleStencilTesting(bool value)
		{
			Submit([value]() { s_RenderApi->ToggleStencilTesting(value); });
		}

		static void inline ToggleFaceCulling(bool value)
		{
			Submit([value]() { s_RenderApi->ToggleFaceCulling(value); });
		}

		static void inline DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			Submit([vertexArray]() { s_RenderApi->DrawIndexed(vertexArray); });
		}

		static void inline ClearColor(const Vec4f& color)
		{
			Submit([color]() { s_RenderApi->ClearColor(color); });
		}

		static void inline Clear()
		{
			Submit([]() { s_RenderApi->Clear(); });
		}

		static void inline SetStencilFunc(uint32_t func, int ref, uint32_t mask)
		{
			Submit([func, ref, mask]() { s_RenderApi->SetStenscilFunc(func, ref, mask); });
		}

		static void SetStencilMask(uint32_t mask) 
		{
			Submit([mask]() {  s_RenderApi->SetStencilMask(mask); });
		}

		static void SetStencilOp(uint32_t sfail, uint32_t dpfail, uint32_t dppass) 
		{
			Submit([sfail, dpfail, dppass]() {  s_RenderApi->SetStencilOp(sfail, dpfail, dppass); });
		}

		static void inline Submit(std::function<void()> function)
		{
			m_Commands.push_back(function);
		}

		static void inline Dispatch()
		{
			if (m_Commands.empty())
				return;

			for (auto& command : m_Commands)
				command();

			m_Commands.clear();
		}

	private:
		static inline Scope<RendererAPI> s_RenderApi = RendererAPI::Create();
		static inline std::vector<std::function<void()>> m_Commands;
	};
}