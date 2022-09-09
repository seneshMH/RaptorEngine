#include "rtpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Raptor {
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



	OpenGLVertexArray::OpenGLVertexArray()
	{
		RT_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		RT_PROFILE_FUNCTION();

		glDeleteVertexArrays(1,&m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		RT_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		RT_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddvertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		RT_PROFILE_FUNCTION();

		RT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		uint32_t m_VertexBufferIndex = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLDataType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLDataType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				RT_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffer.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		RT_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}
