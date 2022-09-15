#pragma once

#include "Raptor/Core/Base.h"
#include "Raptor/Core/Log.h"

#ifdef RT_ENABLE_ASSERTS

namespace Raptor::Assert
{
	constexpr const char* CurrentFileName(const char* path) 
	{
		const char* file = path;
		while (*path)
		{
			if (*path == '/' || *path == '\\')
				file = ++path;
			else
				path++;
		}
		return file;
	}
}

#define RT_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { RT##type##ERROR(msg, __VA_ARGS__); RT_DEBUGBREAK(); } }
#define RT_INTERNAL_ASSERT_WITH_MSG(type, check, ...) RT_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define RT_INTERNAL_ASSERT_NO_MSG(type, check) RT_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", RT_STRINGIFY_MACRO(check), ::Raptor::Assert::CurrentFileName(__FILE__), __LINE__)

#define RT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define RT_INTERNAL_ASSERT_GET_MACRO(...) RT_EXPAND_MACRO( RT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RT_INTERNAL_ASSERT_WITH_MSG, RT_INTERNAL_ASSERT_NO_MSG) )

#define RT_ASSERT(...) RT_EXPAND_MACRO( RT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define RT_CORE_ASSERT(...) RT_EXPAND_MACRO( RT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define RT_ASSERT(...)
#define HZ_CORE_ASSERT(...)
#endif