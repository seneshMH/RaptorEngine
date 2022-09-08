#include "rtpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLTexture.h"

namespace Raptor {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RT_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
			break;
		default:
			RT_CORE_ASSERT(false, "Unkown renderAPI")
				return nullptr;
			break;
		}
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RT_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(filepath);
			break;
		default:
			RT_CORE_ASSERT(false, "Unkown renderAPI")
				return nullptr;
			break;
		}
	}
}