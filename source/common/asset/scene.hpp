#pragma once

#include <filesystem>
#include <unordered_map>

#include <common/core/id.hpp>

namespace pspedit {

struct scene_asset {
	u16 version = 1;
	std::vector<entity_id> entities = {};
	std::unordered_map<entity_id, model_id> models = {};
	std::unordered_map<entity_id, transform_id> transforms = {};
};

template <typename Archive>
void serialize(Archive& archive, scene_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, scene_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const scene_asset& asset);

}