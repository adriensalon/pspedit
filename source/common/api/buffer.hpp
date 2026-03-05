#pragma once

#include <api/vertex.hpp>

namespace pspedit {

struct vram_allocator;

enum struct buffer_usage : u8 {
    static_draw,
    dynamic_draw,
};

struct vertex_buffer_descriptor {
    vertex_descriptor vertex = {};
    buffer_usage usage = buffer_usage::static_draw;
    u32 count = 0;
};

struct index_buffer_descriptor {
    buffer_usage usage = buffer_usage::static_draw;
    u32 count = 0;
};

}