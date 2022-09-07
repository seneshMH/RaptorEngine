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
	m_SquareVA = Raptor::VertexArray::Create();

	float squreVertices[3 * 4] = {
		-0.5f,-0.5f, 0.0f, 
		 0.5f,-0.5f, 0.0f, 
		 0.5f, 0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f
	};

	Raptor::Ref<Raptor::VertexBuffer> squreVB;
	squreVB.reset(Raptor::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));

	Raptor::BufferLayout squreVBLayout = {
		{Raptor::ShaderDataType::Float3,"a_Position"},
	};

	squreVB->SetLayout(squreVBLayout);

	m_SquareVA->AddvertexBuffer(squreVB);


	unsigned int squreIndices[6] = { 0,1,2,2,3,0 };

	Raptor::Ref<Raptor::IndexBuffer> squreIB;
	squreIB.reset(Raptor::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));

	m_SquareVA->SetIndexBuffer(squreIB);
	m_FlatColorShader = Raptor::Shader::Create("assets/shaders/FlatColor.glsl");

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Raptor::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Raptor::RenderCommand::Clear();

	Raptor::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Raptor::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Raptor::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("uColor", m_SquareColor);


	Raptor::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Raptor::Renderer::EndScene();
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
