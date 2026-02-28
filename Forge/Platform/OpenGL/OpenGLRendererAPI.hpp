#pragma once

#include "Renderer/RendererAPI.hpp"

namespace fg
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetViewPort(const Vec2u& lowerLeft, const Vec2u& size) override;
		void ToggleDepthTesting(bool value) override;
		void ToggleFaceCulling(bool value) override;
		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		void ClearColor(const Vec4f& color) override;
		void Clear() override;

		~OpenGLRendererAPI();
	private:
		bool m_isDepthTesting;
	};
}