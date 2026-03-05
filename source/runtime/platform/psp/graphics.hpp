#pragma once

namespace pspedit {

struct render_target {
    int width = 480;
    int height = 272;
    int bufferWidth = 512;
    // int pixelFormat = GU_PSM_8888;

    void* draw_buffer = nullptr;
    void* display_buffer = nullptr;
    void* depth_buffer = nullptr;

    int offsetX = 2048 - (480 / 2);
    int offsetY = 2048 - (272 / 2);
};

struct graphics_frame;

struct graphics_context {
    graphics_context(void* command_list, const std::size_t command_list_bytes);
    graphics_context(const graphics_context& other) = delete;
    graphics_context& operator=(const graphics_context& other) = delete;
    graphics_context(graphics_context&& other) = default;
    graphics_context& operator=(graphics_context&& other) = default;
	~graphics_context();

    [[nodiscard]] graphics_frame begin_frame(const render_target& target);
    void finish_and_sync();
    void swap_buffers();
    void clear(const std::uint32_t clearFlags, const std::uint32_t color = 0, const float depth = 1.0f, const std::uint32_t stencil = 0);
    void setup_viewport(const render_target& target);
    void bind_render_target(const render_target& target);

private:
    void* _command_list;
    std::size_t _command_list_bytes;
    friend struct graphics_frame;
};

struct graphics_frame {
};

}