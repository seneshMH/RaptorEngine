#include "rtpch.h"
#include "Scene.h"
#include "Component.h"

#include "Raptor/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Raptor {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(),this };
		entity.AddCompnent<TransformComponent>();

		auto& tag = entity.AddCompnent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto& entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

}