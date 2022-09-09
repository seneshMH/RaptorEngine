#include "rtpch.h"
#include "Window.h"

#ifdef RT_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Raptor {

	Scope<Window> Window::Create(const WindowProps& props)
	{
		#ifdef RT_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			RT_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}

}