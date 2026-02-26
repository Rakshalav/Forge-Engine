#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"
#include "../Source/Debug/Log.hpp"

namespace fg
{
	static void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei legnth, const char* message, const void* userParam)
	{
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        const char* src = "Unknown";
        const char* typ = "Unknown";

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             src = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   src = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     src = "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     src = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           src = "Other"; break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               typ = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typ = "Deprecated"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typ = "Undefined"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typ = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typ = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              typ = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typ = "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typ = "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               typ = "Other"; break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            FG_CORE_CRITICAL("[OpenGL][{}][{}] {}", src, typ, message);
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            FG_CORE_ERROR("[OpenGL][{}][{}] {}", src, typ, message);
            break;

        case GL_DEBUG_SEVERITY_LOW:
            FG_CORE_WARN("[OpenGL][{}][{}] {}", src, typ, message);
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            FG_CORE_TRACE("[OpenGL][{}][{}] {}", src, typ, message);
            break;
        }
	}

	void OpenGLRendererAPI::Init()
	{
        #if _DEBUG
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        #endif

        glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewPort(const Vec2u& lowerLeft, const Vec2u& size)
	{
		glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);
	}

	void OpenGLRendererAPI::ToggleDepthTesting(bool value)
	{
		value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		value = m_isDepthTesting;
	}

    void OpenGLRendererAPI::ToggleFaceCulling(bool value)
    {
        if (value)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }

        else glDisable(GL_CULL_FACE);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        GLsizei count = static_cast<GLsizei>(vertexArray->GetIndexBuffer()->GetCount());
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }

	void OpenGLRendererAPI::ClearColor(const Vec4f& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		m_isDepthTesting ? glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) : glClear(GL_COLOR_BUFFER_BIT);
	}
}