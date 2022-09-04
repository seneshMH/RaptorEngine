#pragma once
#include "Raptor/Renderer/VertexArray.h"


namespace Raptor {
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddvertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffer; };
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffers() const override { return m_IndexBuffer; };

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	};
}

