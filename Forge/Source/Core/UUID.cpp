#include "UUID.hpp"
#include <random>

namespace fg {
    static uint64_t GenerateInternal() 
    {
        thread_local std::random_device rd;
        thread_local std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> distribute;
        return distribute(gen);
    }

    UUID::UUID() : m_UUID(GenerateInternal()) {}

    UUID::UUID(uint64_t uuid) : m_UUID(uuid) {}
}