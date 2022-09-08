#pragma once
#include <Raptor.h>

class Sandbox2D : public Raptor::Layer
{
public:

	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Raptor::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Raptor::Event& e) override;

private:
	Raptor::OrthographicCameraController m_CameraController;

	Raptor::Ref<Raptor::Shader> m_FlatColorShader;
	Raptor::Ref<Raptor::VertexArray> m_SquareVA;

	Raptor::Ref<Raptor::Texture2D> m_CheckerBordTexture;

	glm::vec4 m_SquareColor = { 0.2f,0.1f,0.8f,1.0f };

};
