#pragma once

#include <common/core/buffer.hpp>

namespace pspedit {

struct vram_allocator;

struct vertex_buffer {
    vertex_buffer(vram_allocator& allocator, const vertex_buffer_descriptor& descriptor);
    vertex_buffer(vram_allocator& allocator, const vertex_buffer_descriptor& descriptor, const void* vertices);
    vertex_buffer(const vertex_buffer&) = delete;
    vertex_buffer& operator=(const vertex_buffer&) = delete;
    vertex_buffer(vertex_buffer&&) noexcept = default;
    vertex_buffer& operator=(vertex_buffer&&) noexcept = default;
    ~vertex_buffer();

    void update(vram_allocator& allocator, const vertex_buffer_descriptor& descriptor, const void* vertices);
    [[nodiscard]] b32 is_valid() const noexcept;
    [[nodiscard]] u32 size_bytes() const noexcept;
    [[nodiscard]] const vertex_buffer_descriptor& descriptor() const noexcept;
    [[nodiscard]] void* data() const noexcept;

private:
    vertex_buffer_descriptor _descriptor;
    void* _data;
    u32 _bytes;
};

struct index_buffer {
    index_buffer(vram_allocator& allocator, const index_buffer_descriptor& descriptor);
    index_buffer(vram_allocator& allocator, const index_buffer_descriptor& descriptor, const u16* indices);
    index_buffer(const index_buffer&) = delete;
    index_buffer& operator=(const index_buffer&) = delete;
    index_buffer(index_buffer&&) noexcept = default;
    index_buffer& operator=(index_buffer&&) noexcept = default;
    ~index_buffer();

    void update(vram_allocator& allocator, const index_buffer_descriptor& descriptor, const u16* indices);
    [[nodiscard]] b32 is_valid() const noexcept;
    [[nodiscard]] u32 size_bytes() const noexcept;
    [[nodiscard]] const index_buffer_descriptor& descriptor() const noexcept;
    [[nodiscard]] void* data() const noexcept;

private:
    index_buffer_descriptor _descriptor;
    void* _data = nullptr;
    u32 _bytes = 0;
};

}