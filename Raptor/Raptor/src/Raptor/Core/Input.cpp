#include "rtpch.h"
#include "Input.h"

#ifdef RT_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Raptor {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
		#ifdef RT_PLATFORM_WINDOWS
			return CreateScope<WindowsInput>();
		#else
			RT_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}

}