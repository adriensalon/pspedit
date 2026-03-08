#pragma once

#include <common/core/scalar.hpp>

namespace pspedit {

enum struct vertex_semantic : u8 {
    position,
    normal,
    color,
    texcoord,
    weights,
    indices
};

enum struct vertex_storage : u8 {
    normalized_u8,
    u16,
    i16,
    f32,
};

struct vertex_attribute {
    vertex_semantic semantic;
    vertex_storage storage;
    u8 components;
    u16 offset;
};

struct vertex_descriptor {
    u16 stride = 0;
    u8 morph_targets = 1;
    u8 skin_weights = 0;
    u8 attributes_count = 0;
    std::array<vertex_attribute, 8> attributes = {};
};

template <typename Archive>
void serialize(Archive& archive, vertex_descriptor& descriptor);

}