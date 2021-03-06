#pragma once

#include "Eos/Core/UID.h"
#include "Eos/Scene/Scene.h"

#include <entt.hpp>

namespace Eos {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene& scene);
		Entity(const Entity&) = default;

		Scene& GetScene() { return *m_Scene; }

		template<typename T, typename... Args>
		Entity& AddComponent(Args&&... args)
		{
			EOS_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return *this;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			EOS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		Entity& RemoveComponent()
		{
			EOS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
			return *this;
		}

		Entity CreateEntity(const std::string_view name = {})
		{
			return m_Scene->CreateEntity(name);
		}

		void DestroyAllEntities()
		{
			m_Scene->DestroyAllEntities();
		}

		void DestroyEntity(Entity entity)
		{
			m_Scene->DestroyEntity(entity);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}
