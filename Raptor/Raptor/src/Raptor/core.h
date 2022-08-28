#pragma once

#ifdef RT_PLATFORM_WINDOWS
	#ifdef RT_BUILD_DLL
		#define RAPTOR_API _declspec(dllexport)
	#else
		#define RAPTOR_API _declspec(dllimport)
	#endif // RT_BUILD_DLL
#else
	#error only suport windows
#endif
