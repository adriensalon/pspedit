#pragma once

#include <api/image.hpp>

namespace pspedit {

struct pipeline_state;

struct render_target {
    u16 width = 480;
    u16 height = 272;
    u16 buffer_width = 512;
    pixel_format format = pixel_format::rgba8888;
    u16 offset_x = 2048 - (480 / 2);
    u16 offset_y = 2048 - (272 / 2);
    void* draw_buffer = nullptr;
    void* display_buffer = nullptr;
    void* depth_buffer = nullptr;
};

struct graphics_context {
    graphics_context(void* command_list, const std::size_t command_list_bytes);
    graphics_context(const graphics_context& other) = delete;
    graphics_context& operator=(const graphics_context& other) = delete;
    graphics_context(graphics_context&& other) noexcept = default;
    graphics_context& operator=(graphics_context&& other) noexcept = default;
	~graphics_context();

    void begin_frame(const render_target& target);
    void end_frame();
    void clear(const u32 clear_flags, const u32 color = 0, const f32 depth = 1.f, const u32 stencil = 0);
	void bind_pipeline(const pipeline_state& pipeline);

private:
    void bind_render_target(const render_target& target);
    void setup_viewport(const render_target& target);
    void finish_and_sync();
    void swap_buffers();

private:
    void* _command_list;
    std::size_t _command_list_bytes;
};

}