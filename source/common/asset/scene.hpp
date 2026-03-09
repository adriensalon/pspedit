#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include <common/core/id.hpp>

namespace pspedit {

struct scene_entity {
	std::optional<model_id> model = std::nullopt;
	std::optional<transform_id> transform = std::nullopt;
};

struct scene_asset {
	u16 version = 1;
	std::vector<scene_entity> entities = {};
};

template <typename Archive>
void serialize(Archive& archive, scene_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, scene_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const scene_asset& asset);

}