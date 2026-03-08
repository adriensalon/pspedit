#pragma once

#include <filesystem>

#include <common/core/id.hpp>

namespace pspedit {

struct model_asset {
	u16 version = 1;
	material_id material = {};
	mesh_id mesh = {};
};

template <typename Archive>
void serialize(Archive& archive, model_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, model_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const model_asset& asset);

}