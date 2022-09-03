#include "rtpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>


namespace Raptor {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
		case Raptor::ShaderDataType::Float:		return GL_FLOAT;
			break;
		case Raptor::ShaderDataType::Float2:	return GL_FLOAT;
			break;
		case Raptor::ShaderDataType::Float3:	return GL_FLOAT;
			break;
		case Raptor::ShaderDataType::Float4:	return GL_FLOAT;
			break;
		case Raptor::ShaderDataType::Mat3:		return GL_FLOAT;
			break;
		case Raptor::ShaderDataType::Mat4:		return GL_FLOAT;
			break;
		case Raptor::ShaderDataType::Int:		return GL_INT;
			break;
		case Raptor::ShaderDataType::Int2:		return GL_INT;
			break;
		case Raptor::ShaderDataType::Int3:		return GL_INT;
			break;
		case Raptor::ShaderDataType::Int4:		return GL_INT;
			break;
		case Raptor::ShaderDataType::Bool:		return GL_BOOL;
			break;
		}

		RT_CORE_ASSERT(false, "Unknown DataType");
		return 0;
	}

	Application::Application()
	{
		RT_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f,-0.5f, 0.0f, 0.8f,0.2f,0.2f,1.0f,
			 0.5f,-0.5f, 0.0f, 0.2f,0.8f,0.2f,1.0f,
			 0.0f, 0.5f, 0.0f, 0.2f,0.2f,0.8f,1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices,sizeof(vertices)));
		
		{
			BufferLayout layout = {
				{ShaderDataType::Float3,"a_Position"},
				{ShaderDataType::Float4,"a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLDataType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset
			);
			index ++;
		}

		
		unsigned int indices[3] = {0,1,2};

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running) 
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
