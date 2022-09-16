#pragma once
#include "Scene.h"

#include <entt.hpp>

namespace Raptor {
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handel,Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T,typename... Args>
		T& AddComponent(Args&&... args)
		{
			RT_CORE_ASSERT(!HasComponent<T>(), "Entity already has component !");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandel, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			RT_CORE_ASSERT(HasComponent<T>(), "Entity not have component !");
			return m_Scene->m_Registry.get<T>(m_EntityHandel);
		}

		template<typename T>
		void RemoveComponent() 
		{
			RT_CORE_ASSERT(HasComponent<T>(), "Entity not have component !");
			m_Scene->m_Registry.remove<T>(m_EntityHandel);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandel);
		}

		operator bool() const { return m_EntityHandel != entt::null; }
		operator entt::entity() const { return m_EntityHandel; }
		operator uint32_t() const { return (uint32_t)m_EntityHandel; }

		bool operator ==(const Entity& other) const { return m_EntityHandel == other.m_EntityHandel && m_Scene == other.m_Scene; }
		bool operator !=(const Entity& other) const { return !(* this == other); }
	private:
		entt::entity m_EntityHandel = entt::null;
		Scene* m_Scene = nullptr;
	};
}