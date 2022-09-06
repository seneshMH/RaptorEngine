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
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1,&m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddvertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		RT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset
			);
			index++;
		}

		m_VertexBuffer.push_back(vertexBuffer);

	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}
