#pragma once

#include "Renderer/RendererAPI.hpp"

namespace fg
{
	class OpenGLRendererAPI final : public RendererAPI
	{
	public:
		void Init() override;
		void SetViewPort(const Vec2u& lowerLeft, const Vec2u& size) override;
		void ToggleDepthTesting(bool value) override;
		void ToggleFaceCulling(bool value) override;
		void ToggleStencilTesting(bool value) override;
		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		void ClearColor(const Vec4f& color) override;
		void Clear() override;

		void SetStenscilFunc(uint32_t func, int ref, uint32_t mask) override;
		void SetStencilMask(uint32_t mask) override;
		void SetStencilOp(uint32_t sfail, uint32_t dpfail, uint32_t dppass) override;

		void SetDepthFunc(uint32_t func) override;

		~OpenGLRendererAPI();

	private:
		bool m_isDepthTesting;
		bool m_StencilTesting;
	};
}