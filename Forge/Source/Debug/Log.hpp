#pragma once

#include <spdlog/spdlog.h>
#include "Core/Base.hpp"

namespace Forge
{
    class Log
    {
    private:
        Log();
        ~Log() = default;

    public:
        static void Init();
        static void UnInit();
        static Log& Get();

        static inline Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static inline Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        static bool InitGlDebugger();

        static void glDebugOutput(
            uint32_t source, 
            uint32_t type, 
            uint32_t id, 
            uint32_t severety,
            uint32_t legnth,
            const char* message, 
            const void* userParam);

    private:
        static inline Ref<spdlog::logger> s_CoreLogger;
        static inline Ref<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define FG_CORE_TRACE(...)    Forge::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FG_CORE_INFO(...)     Forge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FG_CORE_WARN(...)     Forge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FG_CORE_ERROR(...)    Forge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FG_CORE_CRITICAL(...) Forge::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define FG_TRACE(...)         Forge::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FG_INFO(...)          Forge::Log::GetClientLogger()->info(__VA_ARGS__)
#define FG_WARN(...)          Forge::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FG_ERROR(...)         Forge::Log::GetClientLogger()->error(__VA_ARGS__)
#define FG_CRITICAL(...)      Forge::Log::GetClientLogger()->critical(__VA_ARGS__)

