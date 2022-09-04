#include "rtpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

namespace Raptor {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RT_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;
		default:
			RT_CORE_ASSERT(false, "Unkown renderAPI")
				return nullptr;
			break;
		}
	}
}