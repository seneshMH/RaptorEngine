#include <Raptor.h>

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
		std::shared_ptr<Raptor::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Raptor::VertexBuffer::Create(vertices, sizeof(vertices)));


		Raptor::BufferLayout layout = {
			{Raptor::ShaderDataType::Float3,"a_Position"},
			{Raptor::ShaderDataType::Float4,"a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddvertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };

		std::shared_ptr<Raptor::IndexBuffer> indexBuffer;
		indexBuffer.reset(Raptor::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SqureVA.reset(Raptor::VertexArray::Create());

		float squreVertices[3 * 4] = {
			-0.5f,-0.5f, 0.0f,
			 0.5f,-0.5f, 0.0f,
			 0.5f, 0.5f, 0.0f,
			 -0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<Raptor::VertexBuffer> squreVB;
		squreVB.reset(Raptor::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));

		Raptor::BufferLayout squreVBLayout = {
			{Raptor::ShaderDataType::Float3,"a_Position"},
		};

		squreVB->SetLayout(squreVBLayout);

		m_SqureVA->AddvertexBuffer(squreVB);


		unsigned int squreIndices[6] = { 0,1,2,2,3,0 };

		std::shared_ptr<Raptor::IndexBuffer> squreIB;
		squreIB.reset(Raptor::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));

		m_SqureVA->SetIndexBuffer(squreIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 aPosition;
			layout(location=1) in vec4 aColor;

			uniform mat4 u_ViewProjection;

			out vec3 vPosition;
			out vec4 vColor;
			
			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = u_ViewProjection * vec4(aPosition,1.0);
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


		m_Shader.reset(new Raptor::Shader(vertexSrc, fragmentSrc));


		std::string BlueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 aPosition;

			uniform mat4 u_ViewProjection;

			out vec3 vPosition;
			
			void main()
			{
				vPosition = aPosition;
				gl_Position = u_ViewProjection * vec4(aPosition,1.0);
			}		
		)";

		std::string BlueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location=0) out vec4 color;

			in vec3 vPosition;
			
			
			void main()
			{
				color = vec4(0.2,0.3,0.8,1.0);
			}		
		)";

		m_BlueShader.reset(new Raptor::Shader(BlueShaderVertexSrc, BlueShaderFragmentSrc));
	}

	void OnUpdate() override
	{
		if (Raptor::Input::IsKeyPressed( RT_KEY_LEFT))
			m_CameraPosition.x -= m_CameraSpeed;
		else if (Raptor::Input::IsKeyPressed(RT_KEY_RIGHT))
			m_CameraPosition.x += m_CameraSpeed;
		if (Raptor::Input::IsKeyPressed(RT_KEY_DOWN))
			m_CameraPosition.y -= m_CameraSpeed;
		else if (Raptor::Input::IsKeyPressed(RT_KEY_UP))
			m_CameraPosition.y += m_CameraSpeed;

		if (Raptor::Input::IsKeyPressed(RT_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		if (Raptor::Input::IsKeyPressed(RT_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;

		Raptor::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raptor::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Raptor::Renderer::BeginScene(m_Camera);

		Raptor::Renderer::Submit(m_BlueShader, m_SqureVA);
		Raptor::Renderer::Submit(m_Shader, m_VertexArray);

		Raptor::Renderer::EndScene();
	}

	void OnEvent(Raptor::Event& event) override
	{
		
	}

private:
	std::shared_ptr<Raptor::Shader> m_Shader;
	std::shared_ptr<Raptor::VertexArray> m_VertexArray;

	std::shared_ptr<Raptor::Shader> m_BlueShader;
	std::shared_ptr<Raptor::VertexArray> m_SqureVA;

	Raptor::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.01f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 1.0f;
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