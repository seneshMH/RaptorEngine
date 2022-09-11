#pragma once
#include <Raptor.h>

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
		OrthographicCameraController m_CameraController;
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Texture2D> m_CheckerBordTexture;
		
		glm::vec2 m_ViewportSize = {0.0f,0.0f};

		glm::vec4 m_SquareColor = { 0.2f,0.1f,0.8f,1.0f };
	};
}
