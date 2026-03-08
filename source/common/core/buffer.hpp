#pragma once

#include <common/core/vertex.hpp>

namespace pspedit {

struct vram_allocator;

enum struct buffer_usage : u8 {
    static_draw,
    dynamic_draw,
};

enum struct index_format : u8 {
    u16,
    u32
};

struct vertex_buffer_descriptor {
    vertex_descriptor vertex = {};
    buffer_usage usage = buffer_usage::static_draw;
    u32 count = 0;
};

struct index_buffer_descriptor {
    buffer_usage usage = buffer_usage::static_draw;
    index_format format;
    u32 count = 0;
};


template <typename Archive>
void serialize(Archive& archive, vertex_buffer_descriptor& descriptor);

template <typename Archive>
void serialize(Archive& archive, index_buffer_descriptor& descriptor);

}