#include "Texture.hpp"
#include "RendererAPI.hpp"

#include "../Platform/OpenGL/OpenGLTexture.hpp"

namespace fg
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::None: return nullptr;

        case RendererAPI::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }
    }
}