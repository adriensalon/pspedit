#pragma once

#include <api/scalar.hpp>

namespace pspedit {

struct render_target {
    i32 width = 480;
    i32 height = 272;
    i32 bufferWidth = 512;
    // int pixelFormat = GU_PSM_8888;
    i32 pixelFormat;

    void* draw_buffer = nullptr;
    void* display_buffer = nullptr;
    void* depth_buffer = nullptr;

    i32 offsetX = 2048 - (480 / 2);
    i32 offsetY = 2048 - (272 / 2);
};

struct graphics_context {
    graphics_context(void* command_list, const std::size_t command_list_bytes);
    graphics_context(const graphics_context& other) = delete;
    graphics_context& operator=(const graphics_context& other) = delete;
    graphics_context(graphics_context&& other) noexcept = default;
    graphics_context& operator=(graphics_context&& other) noexcept = default;
	~graphics_context();

    void begin_frame(const render_target& target);
    void clear(const u32 clearFlags, const u32 color = 0, const f32 depth = 1.f, const u32 stencil = 0);
    void end_frame();

    void bind_render_target(const render_target& target);
    void setup_viewport(const render_target& target);
    void finish_and_sync();
    void swap_buffers();

private:
    void* _command_list;
    std::size_t _command_list_bytes;
    friend struct graphics_frame;
};

}