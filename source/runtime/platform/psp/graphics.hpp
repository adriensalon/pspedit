#pragma once

#include <api/texture.hpp>

namespace pspedit {

struct pipeline_state;
struct framebuffer;

struct graphics_context {
    graphics_context(framebuffer& target, void* command_list, const std::size_t command_list_bytes);
    graphics_context(const graphics_context& other) = delete;
    graphics_context& operator=(const graphics_context& other) = delete;
    graphics_context(graphics_context&& other) noexcept = default;
    graphics_context& operator=(graphics_context&& other) noexcept = default;
	~graphics_context();

    void begin_frame(const framebuffer& target);
    void end_frame(framebuffer& target);
    void clear(const u32 clear_flags, const u32 color = 0, const f32 depth = 1.f, const u32 stencil = 0);


private:
    void* _command_list;
    std::size_t _command_list_bytes;
};

}