#pragma once

#include <filesystem>
#include <vector>

#include <common/asset/image.hpp>

namespace pspedit {

struct texture;

struct runtime_image_header {
	u32 magic = 0;
    u16 version = 1;
    image_id id = 0;
    texture_descriptor texture = {};
	std::vector<u8> pixels;
};

struct image {
    image(vram_allocator& allocator, const image_asset& asset);

private:
    texture _texture;
};

}