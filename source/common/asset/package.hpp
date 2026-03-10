#pragma once

#include <filesystem>
#include <unordered_map>

#include <common/core/id.hpp>

namespace pspedit {

struct package_asset {
    u16 version = 1;
    std::unordered_map<image_id, std::filesystem::path> images = {};
    std::unordered_map<mesh_id, std::filesystem::path> meshes = {};
    std::unordered_map<material_id, std::filesystem::path> materials = {};
    std::unordered_map<model_id, std::filesystem::path> models = {};
    std::unordered_map<transform_id, std::filesystem::path> transforms = {};
    std::unordered_map<scene_id, std::filesystem::path> scenes = {};
};

template <typename Archive>
void serialize(Archive& archive, package_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, package_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const package_asset& asset);

}