#pragma once

#include <stdint.h>
#include <compare>

namespace fg
{
	struct UUID
	{
		UUID();
		UUID(uint64_t uuid);

		explicit operator uint64_t() const { return m_UUID; }
		auto operator<=>(const UUID&) const = default;

	private:
		uint64_t m_UUID;
	};
}

namespace std 
{
	template <typename T> struct hash;
	template <>
	struct hash<fg::UUID> 
	{
		size_t operator()(const fg::UUID& uuid) const {
			return std::hash<uint64_t>{}((uint64_t)uuid);
		}
	};
}