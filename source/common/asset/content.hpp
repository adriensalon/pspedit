#pragma once

#include <filesystem>
#include <unordered_map>

#include <common/core/id.hpp>

namespace pspedit {

struct content_asset {
    u16 version = 1;
    std::unordered_map<package_id, std::filesystem::path> packages = {};
    package_id main_package = {};
    scene_id main_scene = {};
};

template <typename Archive>
void serialize(Archive& archive, content_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, content_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const content_asset& asset);

}