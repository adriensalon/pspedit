#pragma once

#include <common/core/texture.hpp>

namespace pspedit {

struct framebuffer_descriptor {
    u16 width = 480;
    u16 height = 272;
    u16 buffer_width = 512;
    pixel_format format = pixel_format::rgba8888;
    u16 offset_x = 2048 - (480 / 2);
    u16 offset_y = 2048 - (272 / 2);
    b32 is_presentable = true;
    b32 has_depth = true;
};

}