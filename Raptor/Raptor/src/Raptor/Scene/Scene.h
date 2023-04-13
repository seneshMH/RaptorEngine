#pragma once
#include <entt.hpp>
#include "Raptor/Core/Timestep.h"
#include "Raptor/Core/UUID.h"
#include "Raptor/Renderer/EditorCamera.h"

class b2World;
namespace Raptor {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnSimiulationStart();
		void OnRuntimeStop();
		void OnSimiulationStop();

		void OnUpdateRunTime(Timestep ts);
		void OnUpdateSimiulation(Timestep ts, EditorCamera& camera);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);
		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		bool IsRunning() { return m_IsRunning; }

		template<typename... Components>
		auto GetAllEntitesWidth()
		{
			return m_Registry.view<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		void OnPhysics2DStart();
		void OnPhysics2DStop();
		void RenderScene(EditorCamera& camera);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		bool m_IsRunning = false;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSrializer;
		friend class SceneHierarchyPanel;
	};

}
