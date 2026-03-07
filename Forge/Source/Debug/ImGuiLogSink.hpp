#pragma once

#include <spdlog/sinks/base_sink.h>
#include <mutex>
#include <vector>
#include <string>

namespace fg
{
    struct LogLine {
        std::string Message;
        spdlog::level::level_enum Level;
    };

    class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex>
    {
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            base_sink<std::mutex>::formatter_->format(msg, formatted);

            // Store BOTH the text and the level
            m_MessageBuffer.push_back({ fmt::to_string(formatted), msg.level });

            if (m_MessageBuffer.size() > 500)
                m_MessageBuffer.erase(m_MessageBuffer.begin());
        }

        void flush_() override {}

    public:
        const std::vector<LogLine>& GetMessages() const { return m_MessageBuffer; }
        void Clear() { std::lock_guard<std::mutex> lock(base_sink<std::mutex>::mutex_); m_MessageBuffer.clear(); }

    private:
        std::vector<LogLine> m_MessageBuffer;
    };
}