#pragma once

#include "Eos/Core/Base.h"
#include "Eos/Core/Log.h"
#include <filesystem>

#ifdef EOS_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define EOS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EOS##type##ERROR(msg, __VA_ARGS__); EOS_DEBUGBREAK(); } }
#define EOS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EOS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define EOS_INTERNAL_ASSERT_NO_MSG(type, check) EOS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EOS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define EOS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define EOS_INTERNAL_ASSERT_GET_MACRO(...) EOS_EXPAND_MACRO( EOS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EOS_INTERNAL_ASSERT_WITH_MSG, EOS_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define EOS_ASSERT(...) EOS_EXPAND_MACRO( EOS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define EOS_CORE_ASSERT(...) EOS_EXPAND_MACRO( EOS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define EOS_ASSERT(...)
#define EOS_CORE_ASSERT(...)
#endif