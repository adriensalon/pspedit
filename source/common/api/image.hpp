#pragma once

#include <api/scalar.hpp>

namespace pspedit {

enum struct pixel_format {
    r8,
    r16,
    r32,
    rgb565,
    rgba4444,
    rgba5551,
    rgba8888
};

enum struct texture_filter : u8 {
    nearest,
    linear
};

enum struct texture_wrap : u8 {
    clamp,
    repeat
};

struct image_descriptor {
    u16 width = 0;
    u16 height = 0;
    u32 stride = 0;
    u8 swizzle = 0;
	pixel_format format = pixel_format::rgba8888;
    texture_filter filter_min = texture_filter::nearest;
    texture_filter filter_mag = texture_filter::nearest;
    texture_wrap wrap_u = texture_wrap::clamp;
    texture_wrap wrap_v = texture_wrap::clamp;
};

}