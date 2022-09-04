#include "rtpch.h"
#include "Application.h"
#include "Input.h"

#include "Raptor/Renderer/Renderer.h"


namespace Raptor {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RT_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f,-0.5f, 0.0f, 0.8f,0.2f,0.2f,1.0f,
			 0.5f,-0.5f, 0.0f, 0.2f,0.8f,0.2f,1.0f,
			 0.0f, 0.5f, 0.0f, 0.2f,0.2f,0.8f,1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices,sizeof(vertices)));
		
		
		BufferLayout layout = {
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddvertexBuffer(vertexBuffer);

		unsigned int indices[3] = {0,1,2};

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SqureVA.reset(VertexArray::Create());

		float squreVertices[3 * 4] = {
			-0.5f,-0.5f, 0.0f, 
			 0.5f,-0.5f, 0.0f, 
			 0.5f, 0.5f, 0.0f,
			 -0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squreVB;
		squreVB.reset(VertexBuffer::Create(squreVertices, sizeof(squreVertices)));

		BufferLayout squreVBLayout = {
			{ShaderDataType::Float3,"a_Position"},
		};

		squreVB->SetLayout(squreVBLayout);

		m_SqureVA->AddvertexBuffer(squreVB);


		unsigned int squreIndices[6] = { 0,1,2,2,3,0 };

		std::shared_ptr<IndexBuffer> squreIB;
		squreIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));

		m_SqureVA->SetIndexBuffer(squreIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 aPosition;
			layout(location=1) in vec4 aColor;

			out vec3 vPosition;
			out vec4 vColor;
			
			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = vec4(aPosition,1.0);
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


		m_Shader.reset(new Shader(vertexSrc,fragmentSrc));


		std::string BlueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 aPosition;

			out vec3 vPosition;
			
			void main()
			{
				vPosition = aPosition;
				gl_Position = vec4(aPosition,1.0);
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

		m_BlueShader.reset(new Shader(BlueShaderVertexSrc, BlueShaderFragmentSrc));
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running) 
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_BlueShader->Bind();
			Renderer::Submit(m_SqureVA);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}


			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
