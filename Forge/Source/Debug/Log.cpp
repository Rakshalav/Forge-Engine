#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace fg
{
    static Log* s_instance = nullptr;

    Log::Log()
    {
        s_instance = this;

        // 1. Create the shared console sink
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("%^[%T] %n | %l: %v%$");

        // 2. Create the ImGui sink and store it in your static member
        s_ClientSink = std::make_shared<ImGuiLogSink>();
        s_ClientSink->set_pattern("%^[%T] %l: %v%$");

        // 3. Core Logger: Use only the console sink
        s_CoreLogger = std::make_shared<spdlog::logger>("FORGE", consoleSink);
        s_CoreLogger->set_level(spdlog::level::trace);
        spdlog::register_logger(s_CoreLogger);

        std::vector<spdlog::sink_ptr> clientSinks = { s_ClientSink }; 
        s_ClientLogger = std::make_shared<spdlog::logger>("APP", clientSinks.begin(), clientSinks.end());
        s_ClientLogger->set_level(spdlog::level::trace);
        spdlog::register_logger(s_ClientLogger);
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