#pragma once

#include <common/asset/image.hpp>

namespace pspedit {

struct image_import {
    image_asset image = {};
	std::string editor_name = {};
    std::filesystem::path import_path = "";
};

bool import_image(const std::filesystem::path& import_path, image_import& import);
bool bake_image(const std::filesystem::path& bake_path, const image_import& import);

}
