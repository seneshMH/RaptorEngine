#include "rtpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Raptor {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}