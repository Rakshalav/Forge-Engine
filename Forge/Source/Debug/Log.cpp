#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace fg
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
}