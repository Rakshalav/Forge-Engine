#include <Renderer/FrameBuffer.hpp>
#include <OpenGL/OpenGLFrameBuffer.hpp>
#include <Renderer/RendererAPI.hpp>

namespace fg
{
	Ref<Framebuffer> fg::Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::None:	return nullptr;
		case RendererAPI::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}
	}
}