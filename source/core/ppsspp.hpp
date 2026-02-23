#pragma once

#include <filesystem>

[[nodiscard]] bool is_ppsspp_found();
void launch_ppsspp_game(const std::filesystem::path& game_directory, const std::vector<std::string>& args = {});
