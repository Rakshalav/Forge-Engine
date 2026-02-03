#include "Texture.hpp"
#include "Renderer.hpp"

#include "../Platform/OpenGL/OpenGLTexture.hpp"

namespace fg
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::None: return nullptr;

        case Renderer::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
            
        case Renderer::API::Vulkan: return nullptr;
        }
    }
}