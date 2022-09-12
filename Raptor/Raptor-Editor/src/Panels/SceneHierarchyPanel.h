#pragma once
#include "Raptor/Core/Base.h"
#include "Raptor/Core/Log.h"

#include "Raptor/Scene/Scene.h"
#include "Raptor/Scene/Entity.h"

namespace Raptor {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;
	};
}
