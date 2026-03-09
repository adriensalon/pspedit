#pragma once

#include <filesystem>
#include <optional>
#include <unordered_map>
#include <vector>


#include <common/asset/image.hpp>
#include <common/asset/material.hpp>
#include <common/asset/mesh.hpp>
#include <common/asset/model.hpp>
#include <common/asset/transform.hpp>
#include <common/asset/scene.hpp>


namespace pspedit {

enum struct project_run_target {
    ppsspp_emulator,
    psp_usblink
};

template <typename Asset>
struct project_asset {
    Asset asset = {};
    std::string editor_name = {};
    std::optional<std::filesystem::path> import_path = std::nullopt;
};

struct editor_project {
    std::unordered_map<image_id, project_asset<image_asset>> images = {};
    std::unordered_map<mesh_id, project_asset<mesh_asset>> meshes = {};
    std::unordered_map<material_id, project_asset<material_asset>> materials = {};
    std::unordered_map<model_id, model_asset> models = {};
    std::unordered_map<transform_id, transform_asset> transforms = {};
    std::unordered_map<scene_id, scene_asset> scenes = {};
    std::optional<image_id> selected_image = std::nullopt;
    std::optional<mesh_id> selected_mesh = std::nullopt;
    std::optional<material_id> selected_material = std::nullopt;
    std::optional<u32> selected_entity = std::nullopt;
    std::optional<u32> selected_scene = std::nullopt;
	
    std::filesystem::path directory = {};
};

inline std::optional<editor_project> current_project = std::nullopt;

void open_directory(const std::filesystem::path& project_directory);
void save_all();
void build_and_run();

}