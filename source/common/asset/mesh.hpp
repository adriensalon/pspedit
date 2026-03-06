#pragma once

#include <common/core/buffer.hpp>

namespace pspedit {

struct mesh_asset_header {
    // meta
    vertex_buffer_descriptor vertex_buffer = {};
    index_buffer_descriptor index_buffer = {};
};

struct mesh_asset {
    mesh_asset_header header = {};
    void* vertices = nullptr;
    void* indices = nullptr;
};

}
