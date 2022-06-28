#pragma once

#include "Eos/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Eos {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
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

// Core log macros
#define EOS_CORE_TRACE(...)		::Eos::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EOS_CORE_INFO(...)		::Eos::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EOS_CORE_WARN(...)		::Eos::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EOS_CORE_ERROR(...)		::Eos::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EOS_CORE_CRITICAL(...)	::Eos::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EOS_TRACE(...)			::Eos::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EOS_INFO(...)			::Eos::Log::GetClientLogger()->info(__VA_ARGS__)
#define EOS_WARN(...)			::Eos::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EOS_ERROR(...)			::Eos::Log::GetClientLogger()->error(__VA_ARGS__)
#define EOS_CRITICAL(...)		::Eos::Log::GetClientLogger()->critical(__VA_ARGS__)
