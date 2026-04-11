#pragma once

#include <cstdint>
#include <compare>
#include <format>
#include <functional> 

namespace fg
{
    struct UUID
    {
        UUID();
        UUID(uint64_t uuid) : m_UUID(uuid) {}

        explicit operator uint64_t() const { return m_UUID; }
        auto operator<=>(const UUID&) const = default;

    private:
        uint64_t m_UUID = 0;
    };
}

namespace std
{
    template <>
    struct hash<fg::UUID>
    {
        size_t operator()(const fg::UUID& uuid) const noexcept {
            return std::hash<uint64_t>{}(static_cast<uint64_t>(uuid));
        }
    };

    template<>
    struct formatter<fg::UUID> : formatter<uint64_t>
    {
        auto format(const fg::UUID& uuid, format_context& ctx) const
        {
            return std::formatter<uint64_t>::format(static_cast<uint64_t>(uuid), ctx);
        }
    };
}