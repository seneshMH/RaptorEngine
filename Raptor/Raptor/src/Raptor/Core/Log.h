#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Raptor {
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//Core Logs
#define RT_CORE_ERROR(...)		::Raptor::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RT_CORE_WARN(...)		::Raptor::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RT_CORE_INFO(...)		::Raptor::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RT_CORE_TRACE(...)		::Raptor::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RT_CORE_CRITICAL(...)	::Raptor::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Logs
#define RT_ERROR(...)			::Raptor::Log::GetClientLogger()->error(__VA_ARGS__)
#define RT_WARN(...)			::Raptor::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RT_INFO(...)			::Raptor::Log::GetClientLogger()->info(__VA_ARGS__)
#define RT_TRACE(...)			::Raptor::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RT_CRITICAL(...)		::Raptor::Log::GetClientLogger()->critical(__VA_ARGS__)

