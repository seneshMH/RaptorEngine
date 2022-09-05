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
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),m_CameraPosition(0.0f)
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


		m_Shader.reset( Raptor::Shader::Create(vertexSrc, fragmentSrc));


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

		m_BlueShader.reset( Raptor::Shader::Create(BlueShaderVertexSrc, BlueShaderFragmentSrc));
	}

	void OnUpdate(Raptor::Timestep ts) override
	{
		
		if (Raptor::Input::IsKeyPressed( RT_KEY_LEFT))
			m_CameraPosition.x -= m_CameraSpeed * ts;
		else if (Raptor::Input::IsKeyPressed(RT_KEY_RIGHT))
			m_CameraPosition.x += m_CameraSpeed * ts;
		if (Raptor::Input::IsKeyPressed(RT_KEY_DOWN))
			m_CameraPosition.y -= m_CameraSpeed * ts;
		else if (Raptor::Input::IsKeyPressed(RT_KEY_UP))
			m_CameraPosition.y += m_CameraSpeed * ts;

		if (Raptor::Input::IsKeyPressed(RT_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Raptor::Input::IsKeyPressed(RT_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raptor::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Raptor::Renderer::BeginScene(m_Camera);

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

		Raptor::Renderer::Submit(m_Shader, m_VertexArray);

		Raptor::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Setings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Raptor::Event& event) override
	{
		
	}

private:
	Raptor::Ref<Raptor::Shader> m_Shader;
	Raptor::Ref<Raptor::VertexArray> m_VertexArray;

	Raptor::Ref<Raptor::Shader> m_BlueShader;
	Raptor::Ref<Raptor::VertexArray> m_SqureVA;

	Raptor::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 30.0f;

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