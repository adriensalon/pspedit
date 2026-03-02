#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

[[nodiscard]] std::optional<std::filesystem::path> open_file_dialog(
    const std::vector<std::pair<std::string, std::string>>& filters,
    const std::filesystem::path& default_path);

[[nodiscard]] std::optional<std::filesystem::path> save_file_dialog(
    const std::vector<std::pair<std::string, std::string>>& filters,
    const std::filesystem::path& default_path);

[[nodiscard]] std::optional<std::filesystem::path> pick_directory_dialog(
    const std::filesystem::path& default_path);
