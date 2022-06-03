#pragma once

#include "Eos/Core/Timestep.h"
#include "Eos/Core/UID.h"
#include "Eos/Renderer/EditorCamera.h"

#include <entt.hpp>

#include <string_view>

class b2World;

namespace Eos {

	class Entity;
	struct CameraComponent;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string_view name = {});
		Entity CreateEntityWithUID(UID uid, const std::string_view name = {});
		void DestroyEntity(Entity entity);
		void DestroyAllEntities();

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }

		void SetName(const std::string& name);
		const std::string& GetName() const { return m_Name; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		std::string m_Name = "Untitled";

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
