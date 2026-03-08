#pragma once

#include <filesystem>
#include <unordered_map>

#include <common/core/id.hpp>

namespace pspedit {

struct game_asset {
    u16 version = 1;
    std::unordered_map<package_id, std::filesystem::path> packages = {};
    std::unordered_map<scene_id, std::filesystem::path> scenes = {};
    package_id main_package = {};
    scene_id main_scene = {};
};

template <typename Archive>
void serialize(Archive& archive, game_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, game_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const game_asset& asset);

}