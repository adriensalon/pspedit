#pragma once

#include <common/core/texture.hpp>

namespace pspedit {

struct pipeline_state;
struct framebuffer;
struct vertex_buffer;

struct swapchain {
    swapchain(framebuffer& target, void* command_list, const std::size_t command_list_bytes);
    swapchain(const swapchain& other) = delete;
    swapchain& operator=(const swapchain& other) = delete;
    swapchain(swapchain&& other) noexcept = default;
    swapchain& operator=(swapchain&& other) noexcept = default;
	~swapchain();

    void begin_frame(const framebuffer& target);
    void end_frame(framebuffer& target);
    void clear(const u32 clear_flags, const u32 color = 0, const f32 depth = 1.f, const u32 stencil = 0);
	void draw(const vertex_buffer& buffer); // TODO go inside model ?


private:
    void* _command_list;
    std::size_t _command_list_bytes;
};

}