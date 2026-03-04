#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

namespace pspedit {

struct vram_allocator {
    vram_allocator();
    vram_allocator(const vram_allocator& other) = delete;
    vram_allocator& operator=(const vram_allocator& other) = delete;
    vram_allocator(vram_allocator&& other) = default;
    vram_allocator& operator=(vram_allocator&& other) = default;

	static void dcache_writeback(const void* pointer, const std::size_t bytes);
	static void dcache_invalidate(const void* pointer, const std::size_t bytes);

    void reset();
    [[nodiscard]] void* allocate(const std::size_t bytes, const std::size_t alignment = 16);
    [[nodiscard]] std::size_t size_bytes() const noexcept;
    [[nodiscard]] std::size_t used_bytes() const noexcept;

    template <typename T>
    [[nodiscard]] T* allocate_typed(const std::size_t count = 1, const std::size_t alignment = alignof(T))
    {
        return static_cast<T*>(allocate(sizeof(T) * count, std::max(alignment, std::size_t(16))));
    }

private:
    std::uint8_t* _base;
    std::size_t _size;
    std::size_t _head;
};

}