#include <glad/glad.h>

#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

static void APIENTRY OpenGLDebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei legnth,
    const char* message,
    const void* userParam
)
{
    Forge::Log::glDebugOutput(
        (uint32_t)source,
        (uint32_t)type,
        id,
        (uint32_t)severity,
        (uint32_t)legnth,
        message,
        userParam
    );
}

namespace Forge
{
    static Log* s_instance = nullptr;

    Log::Log()
    {
        s_instance = this;

        s_CoreLogger = spdlog::stdout_color_mt("FORGE");
        s_ClientLogger = spdlog::stdout_color_mt("APP");

        s_CoreLogger->set_level(spdlog::level::trace);
        s_ClientLogger->set_level(spdlog::level::trace);

        s_CoreLogger->set_pattern("%^[%T] %n | %l: %v%$");
        s_ClientLogger->set_pattern("%^[%T] %n | %l: %v%$");
    }

    void Log::UnInit()
    {
        s_instance = nullptr;
    }

    void Log::Init()
    {
        Get(); 
    }

    Log& Log::Get()
    {
        static Log instance; 
        return instance;
    }

    bool Log::InitGlDebugger()
    {
#ifdef _DEBUG
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            return true;
        }
#endif
        return false;
    }

    void Log::glDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, uint32_t legnth, const char* message, const void* userParam)
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
}