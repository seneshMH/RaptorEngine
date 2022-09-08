#include "ExampleLayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>


ExampleLayer::ExampleLayer()
	:Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
{
}

void ExampleLayer::OnAttach()
{
	m_VertexArray = Raptor::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f,-0.5f, 0.0f, 0.8f,0.2f,0.2f,1.0f,
		 0.5f,-0.5f, 0.0f, 0.2f,0.8f,0.2f,1.0f,
		 0.0f, 0.5f, 0.0f, 0.2f,0.2f,0.8f,1.0f
	};
	Raptor::Ref<Raptor::VertexBuffer> vertexBuffer = Raptor::VertexBuffer::Create(vertices, sizeof(vertices));

	Raptor::BufferLayout layout = {
		{Raptor::ShaderDataType::Float3,"a_Position"},
		{Raptor::ShaderDataType::Float4,"a_Color"}
	};

	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddvertexBuffer(vertexBuffer);

	unsigned int indices[3] = { 0,1,2 };

	Raptor::Ref<Raptor::IndexBuffer> indexBuffer = Raptor::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SqureVA = Raptor::VertexArray::Create();

	float squreVertices[5 * 4] = {
		-0.5f,-0.5f, 0.0f,  0.0f,0.0f,
		 0.5f,-0.5f, 0.0f,  1.0f,0.0f,
		 0.5f, 0.5f, 0.0f,  1.0f,1.0f,
		-0.5f, 0.5f, 0.0f,  0.0f,1.0f
	};

	Raptor::Ref<Raptor::VertexBuffer> squreVB = Raptor::VertexBuffer::Create(squreVertices, sizeof(squreVertices));

	Raptor::BufferLayout squreVBLayout = {
		{Raptor::ShaderDataType::Float3,"a_Position"},
		{Raptor::ShaderDataType::Float2,"a_TexCoords"},
	};

	squreVB->SetLayout(squreVBLayout);

	m_SqureVA->AddvertexBuffer(squreVB);


	unsigned int squreIndices[6] = { 0,1,2,2,3,0 };

	Raptor::Ref<Raptor::IndexBuffer> squreIB = Raptor::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));

	m_SqureVA->SetIndexBuffer(squreIB);

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 aPosition;
			layout(location=1) in vec4 aColor;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 vPosition;
			out vec4 vColor;
			
			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = u_ViewProjection * u_Transform * vec4(aPosition,1.0);
			}		
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location=0) out vec4 color;

			in vec3 vPosition;
			in vec4 vColor;
			
			void main()
			{
				//color = vec4(vPosition * 0.5 + 0.5,1.0);
				color = vColor;
			}		
		)";


	m_Shader = Raptor::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);


	std::string BlueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 aPosition;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 vPosition;
			
			void main()
			{
				vPosition = aPosition;
				gl_Position = u_ViewProjection * u_Transform * vec4(aPosition,1.0);
			}		
		)";

	std::string BlueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location=0) out vec4 color;

			in vec3 vPosition;
			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color,1.0);
			}		
		)";

	m_BlueShader = Raptor::Shader::Create("FlatColor", BlueShaderVertexSrc, BlueShaderFragmentSrc);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Raptor::Texture2D::Create("assets/images/checker.png");
	m_LogoTexture = Raptor::Texture2D::Create("assets/images/logo.png");

	textureShader->Bind();
	textureShader->SetInt("uTexture", 0);
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Raptor::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Raptor::RenderCommand::Clear();

	Raptor::Renderer::BeginScene(m_CameraController.GetCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_BlueShader->Bind();
	m_BlueShader->SetFloat3("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Raptor::Renderer::Submit(m_BlueShader, m_SqureVA, transform);
		}
	}

	auto textureShader = m_ShaderLibrary.Get("Texture");

	m_Texture->Bind();
	Raptor::Renderer::Submit(textureShader, m_SqureVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_LogoTexture->Bind();
	Raptor::Renderer::Submit(textureShader, m_SqureVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//Raptor::Renderer::Submit(m_Shader, m_VertexArray);

	Raptor::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Setings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Raptor::Event& e)
{
	m_CameraController.OnEvevnt(e);
}
