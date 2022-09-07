#include "Sandbox2D.h"

#include "Platform/OpenGl/OpenGLShader.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Raptor::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Raptor::RenderCommand::Clear();

	Raptor::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Raptor::Renderer2D::DrawQuad({-1.0f,0.0f},{0.8f,0.8f},m_SquareColor);
	Raptor::Renderer2D::DrawQuad({0.5f,-0.5f},{0.5f,0.75f},m_SquareColor);

	Raptor::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raptor::Event& e)
{
	m_CameraController.OnEvevnt(e);
}
