#pragma once

#include <common/core/mesh.hpp>
#include <runtime/core/buffer.hpp>

namespace pspedit {

struct vram_allocator;

struct mesh {
    mesh(vram_allocator& allocator, const mesh_asset& asset);
    mesh(const mesh& other) = delete;
    mesh& operator=(const mesh& other) = delete;
    mesh(mesh&& other) noexcept = default;
    mesh& operator=(mesh&& other) noexcept = default;

private:
    mesh_asset_header _asset_header;
    vertex_buffer _vertex_buffer;
    index_buffer _index_buffer;
};

}