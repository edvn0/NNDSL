#pragma once

#include <filesystem>
#include <string>

namespace NNDSL::IO {

	std::string read_file(const std::filesystem::path& path);

}