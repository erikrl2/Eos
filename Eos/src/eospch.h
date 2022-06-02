#pragma once

#include "Eos/Core/PlatformDetection.h"

#ifdef EOS_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Eos/Core/Base.h"

#include "Eos/Core/Log.h"

#include "Eos/Debug/Instrumentor.h"

#ifdef EOS_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
