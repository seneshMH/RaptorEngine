#include "rtpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Raptor {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}