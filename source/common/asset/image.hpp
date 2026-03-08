#pragma once

#include <filesystem>
#include <vector>

#include <common/core/texture.hpp>

namespace pspedit {

struct image_asset {
    u16 version = 1;
    texture_descriptor texture = {};
    std::vector<u8> pixels = {};
};

template <typename Archive>
void serialize(Archive& archive, image_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, image_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const image_asset& asset);

}
