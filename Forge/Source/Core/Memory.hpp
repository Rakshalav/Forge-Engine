#pragma once
#include <cstdint>
#include <utility>
#include <atomic>
#include <memory>

namespace fg
{
    class RefCounted
    {
    public:
        RefCounted() : m_RefCount(0) {}
        RefCounted(const RefCounted&) = delete;
        RefCounted& operator=(const RefCounted&) = delete;

        void AddRef() const
        {
            m_RefCount.fetch_add(1, std::memory_order_relaxed);
        }

        bool Release() const
        {
            if (m_RefCount.fetch_sub(1, std::memory_order_release) == 1)
            {
                std::atomic_thread_fence(std::memory_order_acquire);
                return true;
            }
            return false;
        }

        uint32_t RefCount() const
        {
            return m_RefCount.load(std::memory_order_relaxed);
        }

        virtual ~RefCounted() = default;

    private:
        mutable std::atomic<uint32_t> m_RefCount;
    };


    template<typename T>
    class Ref
    {
    public:
        Ref() : m_Ptr(nullptr) {}

        explicit Ref(T* ptr) : m_Ptr(ptr)
        {
            if (m_Ptr) m_Ptr->AddRef();        
        }

        Ref(const Ref& other) : m_Ptr(other.m_Ptr)
        {
            if (m_Ptr) m_Ptr->AddRef();       
        }

        template<typename U>
        Ref(const Ref<U>& other) : m_Ptr(other.m_Ptr)
        {
            if (m_Ptr) m_Ptr->AddRef();        
        }

        Ref(Ref&& other) noexcept : m_Ptr(other.m_Ptr)
        {
            other.m_Ptr = nullptr;
        }

        template<typename U>
        Ref(Ref<U>&& other) noexcept : m_Ptr(other.m_Ptr)
        {
            other.m_Ptr = nullptr;
        }

        Ref(std::nullptr_t) : m_Ptr(nullptr) {}

        ~Ref() { Reset(); }

        Ref& operator=(const Ref& other)
        {
            if (this != &other)
                Ref(other).Swap(*this);
            return *this;
        }

        Ref& operator=(Ref&& other) noexcept   
        {
            Ref tmp(std::move(other));
            tmp.Swap(*this);
            return *this;
        }

        Ref& operator=(std::nullptr_t)
        {
            Reset();                          
            return *this;
        }

        T* Get()                 const { return m_Ptr; }
        T& operator*()           const { return *m_Ptr; }
        T* operator->()          const { return m_Ptr; }
        explicit operator bool() const { return m_Ptr != nullptr; }

        void Reset()
        {
            if (m_Ptr && m_Ptr->Release())
                delete m_Ptr;
            m_Ptr = nullptr;
        }

        void Swap(Ref& other) noexcept
        {
            T* tmp = m_Ptr;
            m_Ptr = other.m_Ptr;
            other.m_Ptr = tmp;
        }

        bool operator==(const Ref& other) const { return m_Ptr == other.m_Ptr; }
        bool operator!=(const Ref& other) const { return m_Ptr != other.m_Ptr; }
        bool operator==(std::nullptr_t)   const { return m_Ptr == nullptr; }
        bool operator!=(std::nullptr_t)   const { return m_Ptr != nullptr; }

    private:
        T* m_Ptr;

        template<typename U>
        friend class Ref;
    };

    template<typename T, typename... Args>
        requires(std::is_base_of_v<RefCounted, T>)
    Ref<T> CreateRef(Args&&... args)
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T, typename U>
        requires(std::is_base_of_v<U, T>)
    Ref<T> StaticRefCast(const Ref<U>& other)
    {
        return Ref<T>(static_cast<T*>(other.Get()));
    }

    template<typename T, typename U>
        requires(std::is_base_of_v<U, T>)
    Ref<T> DynamicRefCast(const Ref<U>& other)
    {
        return Ref<T>(dynamic_cast<T*>(other.Get()));
    }

    template<class T>
    using Scope = std::unique_ptr<T>;
    template<class T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}