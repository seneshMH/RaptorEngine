#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name,Fn&& func)
		:m_Name(name),m_Stoped(false),m_Func(func)
	{
		m_Start_Timepoint = std::chrono::high_resolution_clock::now();
	}

	void Stop() 
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start_Timepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		m_Stoped = true;

		float duration = (end - start) * 0.001f;

		m_Func({ m_Name,duration });

	}

	~Timer()
	{
		if (!m_Stoped)
			Stop();
	}	
private:
	std::chrono::time_point<std::chrono::steady_clock> m_Start_Timepoint;
	bool m_Stoped;
	const char* m_Name;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResult.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerBordTexture = Raptor::Texture2D::Create("assets/images/checker.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Raptor::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		PROFILE_SCOPE("Render prep");
		Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raptor::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Render Draw");
		Raptor::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Raptor::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, m_SquareColor);
		Raptor::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, m_SquareColor);
		Raptor::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.14f }, { 10.0f,10.0f }, m_CheckerBordTexture);

		Raptor::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResult)
	{
		char lable[50];
		strcpy_s(lable, "%.3fms  ");
		strcat_s(lable, result.Name);

		ImGui::Text(lable, result.Time);
	}
	m_ProfileResult.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Raptor::Event& e)
{
	m_CameraController.OnEvevnt(e);
}
