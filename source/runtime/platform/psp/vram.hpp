#pragma once

#include <memory>

namespace pspedit {

struct vram_object {
    vram_object(const std::size_t total_bytes = 0);
    vram_object(const vram_object& other) = delete;
    vram_object& operator=(const vram_object& other) = delete;
    vram_object(vram_object&& other) = default;
    vram_object& operator=(vram_object&& other) = default;
    ~vram_object();

    [[nodiscard]] void* allocate(const std::size_t bytes, const std::size_t align = 16);
    [[nodiscard]] std::size_t mark() const noexcept;
    void reset(const std::size_t marker) noexcept;
    [[nodiscard]] void* base() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::size_t used() const noexcept;
    [[nodiscard]] std::size_t available() const noexcept;
    [[nodiscard]] std::size_t offset_of(address ptr) const noexcept;

private:
    struct vram_object_implementation;
    std::shared_ptr<vram_object_implementation> _implementation;
};

}