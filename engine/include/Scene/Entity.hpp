#pragma once

#include "Scene.hpp"

namespace fg
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(uint32_t id, Scene* scene) : m_ID(id), m_Scene(scene) {}

		template<typename T, typename... Args>
		inline void AddComponent(Args&&... args)
		{
			m_Scene->m_Registery.AddComponent<T>(m_ID, std::forward<Args>(args)...);
		}

		template<typename T>
		inline T& GetComponent() { return m_Scene->m_Registery.GetComponent<T>(m_ID); }

		template<typename T>
		inline bool HasComponent() { return m_Scene->m_Registery.HasComponent<T>(m_ID); }

		operator uint32_t() const { return m_ID; }

	private:
		uint32_t m_ID;
		Scene* m_Scene;
	};
}