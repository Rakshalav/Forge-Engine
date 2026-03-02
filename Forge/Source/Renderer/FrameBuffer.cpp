#include "FrameBuffer.hpp"
#include "../Platform/OpenGL/OpenGLFrameBuffer.hpp"
#include "RendererAPI.hpp"

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