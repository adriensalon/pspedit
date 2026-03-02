#pragma once

#include <vector>

#include <core/graphics.hpp>
#include <core/id.hpp>
#include <platform/psp/texture.hpp>

namespace pspedit {

struct image_object {
    u16 width;
    u16 height;
    u32 stride_width;
    texture_internal_format format;
    texture_filter min_filter;
    texture_filter mag_filter;
    u8 swizzle;
    std::vector<u8> pixels;
};

struct image_object_reference {
    object_id id;
};

struct runtime_image {
    runtime_image(const psp_texture_descriptor& descriptor);
    runtime_image(const runtime_image& other) = delete;
    runtime_image& operator=(const runtime_image& other) = delete;
    runtime_image(runtime_image&& other) = default;
    runtime_image& operator=(runtime_image&& other) = default;
    ~runtime_image();

    runtime_id id;
    psp_texture_object psp_texture;
};

struct shared_image {
    runtime_id id = null_runtime_id;
};

}