#include "rtpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGl/OpenGlBuffer.h"

namespace Raptor {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None :
			RT_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL :
			return CreateRef<OpenGLVertexBuffer>(vertices,size);
			break;
		default:
			RT_CORE_ASSERT(false,"Unkown renderAPI")
			return nullptr;
			break;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RT_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
			break;
		default:
			RT_CORE_ASSERT(false, "Unkown renderAPI")
				return nullptr;
			break;
		}
	}
}
