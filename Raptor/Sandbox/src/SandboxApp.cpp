#include <Raptor.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGl/OpenGLShader.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

// enable optimus!
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class ExampleLayer : public Raptor::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.0f/720.0f,true)
	{
		m_VertexArray.reset(Raptor::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f,-0.5f, 0.0f, 0.8f,0.2f,0.2f,1.0f,
			 0.5f,-0.5f, 0.0f, 0.2f,0.8f,0.2f,1.0f,
			 0.0f, 0.5f, 0.0f, 0.2f,0.2f,0.8f,1.0f
		};
		Raptor::Ref<Raptor::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Raptor::VertexBuffer::Create(vertices, sizeof(vertices)));


		Raptor::BufferLayout layout = {
			{Raptor::ShaderDataType::Float3,"a_Position"},
			{Raptor::ShaderDataType::Float4,"a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddvertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };

		Raptor::Ref<Raptor::IndexBuffer> indexBuffer;
		indexBuffer.reset(Raptor::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SqureVA.reset(Raptor::VertexArray::Create());

		float squreVertices[5 * 4] = {
			-0.5f,-0.5f, 0.0f,  0.0f,0.0f,
			 0.5f,-0.5f, 0.0f,  1.0f,0.0f,
			 0.5f, 0.5f, 0.0f,  1.0f,1.0f,
			-0.5f, 0.5f, 0.0f,  0.0f,1.0f
		};

		Raptor::Ref<Raptor::VertexBuffer> squreVB;
		squreVB.reset(Raptor::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));

		Raptor::BufferLayout squreVBLayout = {
			{Raptor::ShaderDataType::Float3,"a_Position"},
			{Raptor::ShaderDataType::Float2,"a_TexCoords"},
		};

		squreVB->SetLayout(squreVBLayout);

		m_SqureVA->AddvertexBuffer(squreVB);


		unsigned int squreIndices[6] = { 0,1,2,2,3,0 };

		Raptor::Ref<Raptor::IndexBuffer> squreIB;
		squreIB.reset(Raptor::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));

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


		m_Shader = Raptor::Shader::Create("VertexPosColor",vertexSrc, fragmentSrc);


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

		m_BlueShader = Raptor::Shader::Create("FlatColor",BlueShaderVertexSrc, BlueShaderFragmentSrc);

		auto textureShader =  m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Raptor::Texture2D::Create("assets/images/checker.png");
		m_LogoTexture = Raptor::Texture2D::Create("assets/images/logo.png");

		std::dynamic_pointer_cast<Raptor::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Raptor::OpenGLShader>(textureShader)->UploadUniformInt("uTexture", 0);
	}

	void OnUpdate(Raptor::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);
		
		Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raptor::RenderCommand::Clear();

		Raptor::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Raptor::OpenGLShader>(m_BlueShader)->Bind();
		std::dynamic_pointer_cast<Raptor::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
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

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Setings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Raptor::Event& e) override
	{
		m_CameraController.OnEvevnt(e);
	}

private:
	Raptor::ShaderLibrary m_ShaderLibrary;
	Raptor::Ref<Raptor::Shader> m_Shader;
	Raptor::Ref<Raptor::VertexArray> m_VertexArray;

	Raptor::Ref<Raptor::Shader> m_BlueShader;
	Raptor::Ref<Raptor::VertexArray> m_SqureVA;

	Raptor::Ref<Raptor::Texture2D> m_Texture,m_LogoTexture;

	Raptor::OrthographicCameraController m_CameraController;
	
	glm::vec3 m_SquareColor = { 0.2f,0.1f,0.8f };

};


class SandBox : public Raptor::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox(){}
};

Raptor::Application* Raptor::CreateApplication()
{
	return new SandBox();
}