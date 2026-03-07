#pragma once

#include <filesystem>

#include <common/core/id.hpp>
#include <common/core/texture.hpp>

namespace pspedit {

struct image_header {
    u32 magic = 0;
    u16 version = 1;
    image_id id = {};
    texture_descriptor texture = {};
};

template <class Archive>
void serialize(Archive& archive, image_header& header);


bool load_image(const std::filesystem::path& archive_path, image_header& header, std::vector<u8>& pixels);
bool save_image(const std::filesystem::path& archive_path, const image_header& header, const std::vector<u8>& pixels);

}
