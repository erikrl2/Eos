#pragma once

#include <string>

namespace Eos {

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	class Platform
	{
	public:
		static float GetTime();
	};

}
