#pragma once
#include "Raptor/Core/Buffer.h"

namespace Raptor {
	class FileSystem {
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
		//static std::string ReadFileText(const std::filesystem::path& filepath);
	};
}
