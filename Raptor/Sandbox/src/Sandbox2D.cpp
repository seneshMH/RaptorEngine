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
	m_SpriteSheet = Raptor::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
	m_TextureStairs = Raptor::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128,128 });
	m_TextureBarrels = Raptor::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8,2 }, { 128,128 });
	m_TextureTree = Raptor::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 },{1,2});

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{
	RT_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Raptor::Timestep ts)
{
	RT_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Raptor::Renderer2D::ResetStats();
	
	{
		RT_PROFILE_SCOPE("Render prep");
		Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raptor::RenderCommand::Clear();
	}

#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Raptor::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Raptor::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f,0.8f }, m_SquareColor);
		Raptor::Renderer2D::DrawQuad({ -0.5f, -0.5f }, { 0.5f,0.75f }, m_SquareColor);
		Raptor::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 20.0f,20.0f }, m_CheckerBordTexture,10.0f);
		Raptor::Renderer2D::DrawRotatedQuad(glm::vec3(0.0f), {1.0f,1.0f}, glm::radians( rotation), m_CheckerBordTexture, 10.0f);
		Raptor::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f }, { 0.5f,0.75f }, glm::radians(-45.0f), m_SquareColor);
		
		Raptor::Renderer2D::EndScene();

		Raptor::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f,0.4f,(y + 5.0f )/ 10.0f,0.7f };
				Raptor::Renderer2D::DrawQuad({ x, y }, { 0.45f,0.45f }, color);
			}
		}

		Raptor::Renderer2D::EndScene();
	}
#endif

	if (Raptor::Input::IsMouseButtonPressed(RT_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Raptor::Input::GetMousePosition();
		auto width = Raptor::Application::Get().GetWindow().GetWidth();
		auto height = Raptor::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	Raptor::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Raptor::Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f},m_TextureStairs);
	Raptor::Renderer2D::DrawQuad({ 1.0f,0.0f,0.0f }, { 1.0f,1.0f},m_TextureBarrels);
	Raptor::Renderer2D::DrawQuad({ -1.0f,0.0f,0.0f }, { 1.0f,2.0f},m_TextureTree);
	Raptor::Renderer2D::EndScene();

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
	RT_PROFILE_FUNCTION();

	ImGui::Begin("Setings");

	auto stats = Raptor::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats :");
	ImGui::Text("Draw Calls	: %d", stats.DrawCalls);
	ImGui::Text("Quads		: %d", stats.QuadCount);
	ImGui::Text("Vertices	: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices		: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raptor::Event& e)
{
	m_CameraController.OnEvevnt(e);
}
