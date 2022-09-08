#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	RT_PROFILE_FUNCTION();

	m_CheckerBordTexture = Raptor::Texture2D::Create("assets/images/checker.png");
}

void Sandbox2D::OnDetach()
{
	RT_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Raptor::Timestep ts)
{
	RT_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);
	
	{
		RT_PROFILE_SCOPE("Render prep");
		Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raptor::RenderCommand::Clear();
	}

	{
		RT_PROFILE_SCOPE("Render Draw");
		Raptor::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Raptor::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, m_SquareColor);
		Raptor::Renderer2D::DrawRotatedQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, glm::radians(-45.0f), m_SquareColor);
		Raptor::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.14f }, { 10.0f,10.0f }, m_CheckerBordTexture,10.0f,glm::vec4(1.0f,0.9f,0.9f,1.0f));
		
		Raptor::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	RT_PROFILE_FUNCTION();

	ImGui::Begin("Setings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raptor::Event& e)
{
	m_CameraController.OnEvevnt(e);
}
