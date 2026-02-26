#pragma once

#include <spdlog/spdlog.h>
#include "Core/Base.hpp"

namespace fg
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

    private:
        static inline Ref<spdlog::logger> s_CoreLogger;
        static inline Ref<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define FG_CORE_TRACE(...)    fg::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FG_CORE_INFO(...)     fg::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FG_CORE_WARN(...)     fg::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FG_CORE_ERROR(...)    fg::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FG_CORE_CRITICAL(...) fg::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define FG_TRACE(...)         fg::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FG_INFO(...)          fg::Log::GetClientLogger()->info(__VA_ARGS__)
#define FG_WARN(...)          fg::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FG_ERROR(...)         fg::Log::GetClientLogger()->error(__VA_ARGS__)
#define FG_CRITICAL(...)      fg::Log::GetClientLogger()->critical(__VA_ARGS__)

