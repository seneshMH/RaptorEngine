#pragma once

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
