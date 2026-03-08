#pragma once

#include <runtime/core/texture.hpp>

namespace pspedit {

struct image_asset;
struct vram_allocator;

struct image {
    image(vram_allocator& allocator, const image_asset& asset);
    image(const image& other) = delete;
    image& operator=(const image& other) = delete;
    image(image&& other) noexcept = default;
    image& operator=(image&& other) noexcept = default;



private:
    texture _texture;
};

}