#pragma once
#include <Raptor.h>

class ExampleLayer : public Raptor::Layer
{
public:

	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Raptor::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Raptor::Event& e) override;

private:

	Raptor::ShaderLibrary m_ShaderLibrary;
	Raptor::Ref<Raptor::Shader> m_Shader;
	Raptor::Ref<Raptor::VertexArray> m_VertexArray;
	Raptor::Ref<Raptor::Shader> m_BlueShader;
	Raptor::Ref<Raptor::VertexArray> m_SqureVA;
	Raptor::Ref<Raptor::Texture2D> m_Texture, m_LogoTexture;
	Raptor::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f,0.1f,0.8f };
};
