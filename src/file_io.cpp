#include "nndsl_pch.hpp"

#include "file_io.hpp"

namespace NNDSL::IO {

	std::string read_file(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::ate | std::ios::in);
		if (!file)
			throw std::exception("Could not open file");

		std::size_t size = file.tellg();
		file.seekg(0);

		std::string buffer(size, ' ');
		file.read(buffer.data(), size);

		return buffer;
	}

} // namespace NNDSL::IO