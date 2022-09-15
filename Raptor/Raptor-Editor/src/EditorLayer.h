#pragma once
#include <Raptor.h>
#include "Panels/SceneHierarchyPanel.h"

#include "Raptor/Renderer/EditorCamera.h"

namespace Raptor {

	class EditorLayer : public Raptor::Layer
	{
	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		OrthographicCameraController m_CameraController;
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Texture2D> m_CheckerBordTexture;

		Ref<Scene> m_ActiveScene;
		Entity m_SqureEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;
		
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_viewportSize = {0.0f,0.0f};
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f,0.1f,0.8f,1.0f };

		int m_GizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
