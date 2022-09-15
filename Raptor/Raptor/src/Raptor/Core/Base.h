#pragma once
#include <memory>

#include "PlatformDetecttion.h"

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

#define RT_EXPAND_MACRO(x) x
#define RT_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)
#define RT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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

#include "Raptor/Core/Log.h"
#include "Raptor/Core/Assert.h"