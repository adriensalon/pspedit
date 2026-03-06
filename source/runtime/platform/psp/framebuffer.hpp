#pragma once

#include <api/framebuffer.hpp>

namespace pspedit {

struct vram_allocator;

struct framebuffer {
    framebuffer(vram_allocator& allocator, const framebuffer_descriptor& descriptor);
    framebuffer(const framebuffer&) = delete;
    framebuffer& operator=(const framebuffer&) = delete;
    framebuffer(framebuffer&&) noexcept = default;
    framebuffer& operator=(framebuffer&&) noexcept = default;
    ~framebuffer();

	[[nodiscard]] static framebuffer default_framebuffer(vram_allocator& allocator);

    [[nodiscard]] b32 is_valid() const noexcept;
    [[nodiscard]] const framebuffer_descriptor& descriptor() const noexcept;
	[[nodiscard]] u32 draw_buffer() const noexcept;
	[[nodiscard]] u32 display_buffer() const noexcept;
	[[nodiscard]] u32 depth_buffer() const noexcept;

private:
    framebuffer_descriptor _descriptor = {};
    void* _draw_buffer = nullptr;
    void* _display_buffer = nullptr;
    void* _depth_buffer = nullptr;
};

}