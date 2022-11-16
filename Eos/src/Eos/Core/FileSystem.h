#pragma once

#include "Eos/Core/Buffer.h"

namespace Eos {

	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}
