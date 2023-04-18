#include "rtpch.h"
#include "FileSystem.h"

namespace Raptor {

	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return {};
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = (uint32_t)(end - stream.tellg());

		if (size == 0)
		{
			// File is empty
			return {};
		}

		Buffer buffer = Buffer(size);
		stream.read(buffer.As<char>(), size);
		stream.close();

		return buffer;
	}

}