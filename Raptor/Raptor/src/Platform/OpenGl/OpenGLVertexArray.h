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

		virtual void AddvertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffer; };
		virtual const Ref<IndexBuffer>& GetIndexBuffers() const override { return m_IndexBuffer; };

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

	};
}

