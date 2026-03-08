#pragma once

#include <filesystem>
#include <unordered_map>

#include <common/asset/transform.hpp>

namespace pspedit {

struct scene_asset {
	u16 version = 1;
	std::unordered_map<entity_id, transform_id> transforms = {};
};

template <typename Archive>
void serialize(Archive& archive, scene_asset& asset);

bool load_scene(const std::filesystem::path& archive_path, scene_asset& asset);
bool save_scene(const std::filesystem::path& archive_path, const scene_asset& asset);

}