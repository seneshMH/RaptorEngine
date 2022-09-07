#pragma once
#include "Raptor/Core/core.h"
#include <memory>
#include "Raptor/Renderer/Buffer.h"

namespace Raptor {
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddvertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffers() const = 0;

		static VertexArray* Create();
	};
}
