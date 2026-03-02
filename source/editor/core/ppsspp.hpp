#pragma once

#include <filesystem>

[[nodiscard]] bool launch_ppsspp_game(const std::filesystem::path& install_directory, const std::vector<std::string>& arguments = {});
