#pragma once

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
    vertex_buffer _vertex_buffer;
    index_buffer _index_buffer;
};

}