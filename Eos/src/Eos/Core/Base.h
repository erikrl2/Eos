#pragma once

#include <memory>

#include "Eos/Core/PlatformDetection.h"

#ifdef EOS_DEBUG
	#if defined(EOS_PLATFORM_WINDOWS)
	#define EOS_DEBUGBREAK() __debugbreak()
	#elif defined(EOS_PLATFORM_LINUX)
	#include <signal.h>
	#define EOS_DEBUGBREAK() raise(SIGTRAP)
	#else
	#error "Platform doesn't support debugbreak yet!"
	#endif
	#define EOS_ENABLE_ASSERTS
#else
	#define EOS_DEBUGBREAK()
#endif


#define BIT(x) (1 << x)

#define EOS_EXPAND_MACRO(x) x
#define EOS_STRINGIFY_MACRO(x) #x

#define EOS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Eos {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	constexpr Scope<T> GetScope(T& ptr)
	{
		return std::unique_ptr<T>(&ptr);
	}

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	constexpr Ref<T> GetRef(T& ptr)
	{
		return std::shared_ptr<T>(&ptr);
	}

	template<typename T1, typename T2>
	constexpr Ref<T1> CastRef(Ref<T2>& ptr)
	{
		return std::static_pointer_cast<T1>(ptr);
	}

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Eos/Core/Log.h"
#include "Eos/Core/Assert.h"
