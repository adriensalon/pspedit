#pragma once

#include <filesystem>

namespace pspedit {

[[nodiscard]] bool is_ppsspp_found();
void launch_ppsspp_game(const std::filesystem::path& game_directory, const std::vector<std::string>& args = {});

}