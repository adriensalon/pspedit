#pragma once

#include <filesystem>
#include <optional>
#include <vector>
#include <unordered_map>

#include <common/core/id.hpp>
#include <editor/asset/image.hpp>
#include <editor/asset/mesh.hpp>

enum struct project_run_target {
    ppsspp_emulator,
    psp_usblink
};

struct editor_project {
	std::unordered_map<pspedit::image_id, pspedit::image_import> images = {};
	std::unordered_map<pspedit::mesh_id, pspedit::mesh_import> meshes = {};
	std::optional<pspedit::image_id> selected_image = std::nullopt;
	std::optional<pspedit::mesh_id> selected_mesh = std::nullopt;
    std::filesystem::path directory = {};
};

inline std::optional<editor_project> current_project = std::nullopt;

void open_directory(const std::filesystem::path& project_directory);
void save_all();
void build_and_run();

