#pragma once

#ifdef RT_PLATFORM_WINDOWS
	#if RT_DYANAMIC_LINK
		#ifdef RT_BUILD_DLL
			#define RAPTOR_API _declspec(dllexport)
		#else
			#define RAPTOR_API _declspec(dllimport)
		#endif // RT_BUILD_DLL
	#else
		#define RAPTOR_API
	#endif
#else
	#error only suport windows
#endif

#ifdef RT_DEBUG
	#define RT_ENABLE_ASSERTS
#endif // RT_DEBUG


#ifdef RT_ENABLE_ASSERTS
	#define RT_ASSERT(x, ...) {if(!(x)) {RT_ERROR("Assertion faild : {0}",__VA_ARGS__); __debugbreak(); }}
	#define RT_CORE_ASSERT(x, ...) {if(!(x)) {RT_CORE_ERROR("Assertion faild : {0}",__VA_ARGS__); __debugbreak(); }}
#else
	#define RT_ASSERT(x,...)
	#define RT_CORE_ASSERT(x,...)
#endif // RT_ENABLE_ASSERTS


#define BIT(x) (1 << x)
#define RT_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)