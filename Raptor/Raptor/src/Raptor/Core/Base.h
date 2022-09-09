#pragma once
#include <memory>

#ifdef _WIN32

	#ifdef _WIN64
		#define RT_PLATFORM_WINDOWS
	#else
		#error only suport windows x64
	#endif // __WIN64

#elif defined(__APPLE__) || defined (__MACH__)

	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR == 1

		#error "ios simulator not supported"

	#elif TARGET_OS_IPHONE == 1

		#define RT_PLATFORM_IOS
		#error "ios not supported"

	#elif TARGET_OS_MAC

		#define RT_PLATFORM_MACOS
		#error "mac os not supported"
	#else
		#error "unknown apple platform"
	#endif 

#elif defined(__ANDROID__)

	#define RT_PLATFORM_ANDROID
	#error "Android is not supported!"

#elif defined(__linux__)

	#define RT_PLATFORM_LINUX
	#error "Linux is not supported!"

#else

	#error "Unknown platform!"

#endif

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
	#if defined(RT_PLATFORM_WINDOWS)
		#define RT_DEBUGBREAK() __debugbreak()
	#elif defined(RT_PLATFORM_LINUX)
		#include <signal.h>
		#define RT_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
		#define RT_ENABLE_ASSERTS
#else
		#define RT_DEBUGBREAK()
#endif // RT_DEBUG


#ifdef RT_ENABLE_ASSERTS
	#define RT_ASSERT(x, ...) {if(!(x)) {RT_ERROR("Assertion faild : {0}",__VA_ARGS__); RT_DEBUGBREAK(); }}
	#define RT_CORE_ASSERT(x, ...) {if(!(x)) {RT_CORE_ERROR("Assertion faild : {0}",__VA_ARGS__); RT_DEBUGBREAK(); }}
#else
	#define RT_ASSERT(x,...)
	#define RT_CORE_ASSERT(x,...)
#endif // RT_ENABLE_ASSERTS


#define BIT(x) (1 << x)
#define RT_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

namespace Raptor {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}