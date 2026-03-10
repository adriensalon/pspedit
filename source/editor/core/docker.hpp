#pragma once

#include <filesystem>

[[nodiscard]] bool docker_build_project(
	const std::filesystem::path& source_directory, 
	const std::filesystem::path& build_directory,
const std::optional<std::filesystem::path>& custom_pspedit_dir = std::nullopt);
