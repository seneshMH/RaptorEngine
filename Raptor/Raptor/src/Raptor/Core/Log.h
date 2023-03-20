#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "Base.h"
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
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

