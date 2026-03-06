#pragma once

#include <common/core/texture.hpp>

namespace pspedit {

struct image_asset_header {
    // meta
    texture_descriptor texture = {};
};

void load_image_asset(const std::filesystem::path& image_path, image_asset_header& header, void* pixels);

}
