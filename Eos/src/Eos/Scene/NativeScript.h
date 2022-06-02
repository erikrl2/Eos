#pragma once

#include "Eos/Scene/Entity.h"

namespace Eos {

	class NativeScript
	{
	public:
		NativeScript(Entity entity) : m_Entity(entity) {}
		virtual ~NativeScript() = default;

		virtual void OnUpdate(Timestep ts) {}

		Scene& GetScene() { return m_Entity.GetScene(); }

		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }

		template<typename T>
		bool HasComponent() const { return m_Entity.HasComponent<T>(); }

		Entity CreateEntity() { return m_Entity.CreateEntity(); }

		void DestroyAllEntities() { return m_Entity.DestroyAllEntities(); }

		void DestroyEntity(Entity entity) { m_Entity.DestroyEntity(entity); }
	private:
		Entity m_Entity;
	};

}
