#pragma once

#include <filesystem>
#include <vector>
#include <optional>

#include <common/core/id.hpp>

namespace pspedit {

struct transform_asset {
	u16 version = 1;
	f32x4x4 trs = {}; // TODO identity
	std::optional<transform_id> parent = std::nullopt;
	std::vector<transform_id> children = {};
};

template <typename Archive>
void serialize(Archive& archive, transform_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, transform_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const transform_asset& asset);

}