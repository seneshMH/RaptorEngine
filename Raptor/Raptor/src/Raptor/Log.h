#pragma once
#include<memory>

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Raptor {
	class RAPTOR_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core Logs
#define RT_CORE_ERROR(...)	::Raptor::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RT_CORE_WARN(...)	::Raptor::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RT_CORE_INFO(...)	::Raptor::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RT_CORE_TRACE(...)	::Raptor::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RT_CORE_FATAL(...)	::Raptor::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Logs
#define RT_ERROR(...)		::Raptor::Log::GetClientLogger()->error(__VA_ARGS__)
#define RT_WARN(...)		::Raptor::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RT_INFO(...)		::Raptor::Log::GetClientLogger()->info(__VA_ARGS__)
#define RT_TRACE(...)		::Raptor::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RT_FATAL(...)		::Raptor::Log::GetClientLogger()->fatal(__VA_ARGS__)

