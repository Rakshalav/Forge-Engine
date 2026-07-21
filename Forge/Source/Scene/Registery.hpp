#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace fg
{
	struct IComponentPool
	{
		virtual ~IComponentPool() = default;
		virtual void Remove(uint32_t entityID) = 0;
	};

	template<typename T>
	struct ComponentPool : public IComponentPool
	{
		template<typename... Args>
		inline T& Add(uint32_t entityID, Args&&... args)
		{
			auto index = static_cast<uint32_t>(m_Components.size());
			m_Components.emplace_back(std::forward<Args>(args)...);
			m_EntityToIndex[entityID] = index;
			m_IndexToEntity.push_back(entityID);
			return m_Components.back();
		}

		inline void Remove(uint32_t entityID) override
		{
			if (!Has(entityID)) return;

			uint32_t indexToRemove = m_EntityToIndex[entityID];
			uint32_t lastEntity = m_IndexToEntity.back();

			if (entityID != lastEntity)
			{
				m_Components[indexToRemove] = std::move(m_Components.back());
				m_IndexToEntity[indexToRemove] = lastEntity;
				m_EntityToIndex[lastEntity] = indexToRemove;
			}

			m_Components.pop_back();
			m_IndexToEntity.pop_back();
			m_EntityToIndex.erase(entityID);
		}

		inline T& operator[](uint32_t entityID)
		{
			return m_Components[m_EntityToIndex[entityID]];
		}

		inline bool Has(uint32_t entityID) const
		{
			return m_EntityToIndex.count(entityID) > 0;
		}

		inline uint32_t GetEntityAt(uint32_t index)
		{
			return m_IndexToEntity[index];
		}

		std::vector<T>& Get() { return m_Components; }

	private:
		std::vector<uint32_t> m_IndexToEntity;
		std::vector<T> m_Components;
		std::unordered_map<uint32_t, uint32_t> m_EntityToIndex;
	};

	class Registery
	{
	public:
		inline uint32_t CreateEntity()
		{
			return m_NextID++;
		}

		template<typename T, typename... Args>
		inline T& AddComponent(uint32_t entityID, Args&&... args)
		{
			return GetComponentPool<T>().Add(entityID, std::forward<Args>(args)...);
		}

		template<typename T>
		inline T& GetComponent(uint32_t entityID)
		{
			return GetComponentPool<T>()[entityID];
		}

		template<typename T>
		bool HasComponent(uint32_t entityID)
		{
			auto it = m_Pools.find(typeid(T));
			if (it == m_Pools.end()) return false;
			return static_cast<ComponentPool<T>*>(it->second.get())->Has(entityID);
		}

		template<typename T>
		void RemoveComponent(uint32_t entityID)
		{
			auto it = m_Pools.find(typeid(T));
			if (it != m_Pools.end())
				it->second->Remove(entityID);
		}

		template<typename T>
		ComponentPool<T>& GetPool()
		{
			return GetComponentPool<T>();
		}

	private:
		template<typename T>
		inline ComponentPool<T>& GetComponentPool()
		{
			std::type_index key(typeid(T));
			if (m_Pools.find(key) == m_Pools.end())
				m_Pools[key] = CreateScope<ComponentPool<T>>();
			return *static_cast<ComponentPool<T>*>(m_Pools[key].get());
		}

	private:
		std::unordered_map<std::type_index, Scope<IComponentPool>> m_Pools;
		uint32_t m_NextID = 0;
	};
}