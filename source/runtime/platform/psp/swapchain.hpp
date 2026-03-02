#pragma once

#include <memory>

#include <pspedit/core/scalar.hpp>
#include <pspedit/psp/command.hpp>

namespace pspedit {

struct swapchain_descriptor {
    u32 width = 480;
    u32 height = 272;
    u32 buffer_width = 512;
    texture_format format = texture_format::rgba5650;
    bool triple_buffer = false;
    bool depth_buffer = false;
};

struct swapchain_object {
    swapchain_object(const swapchain_descriptor& descriptor);
    swapchain_object(const swapchain_object& other) = delete;
    swapchain_object& operator=(const swapchain_object& other) = delete;
    swapchain_object(swapchain_object&& other) = default;
    swapchain_object& operator=(swapchain_object&& other) = default;
    ~swapchain_object();

    void begin_frame(command_object& command);
    void end_frame(command_object& command);
    void present();
    [[nodiscard]] void* color_buffer() const noexcept;
    [[nodiscard]] void* depth_buffer() const noexcept;
    [[nodiscard]] u32 width() const noexcept;
    [[nodiscard]] u32 height() const noexcept;
    [[nodiscard]] u32 buffer_width() const noexcept;
    [[nodiscard]] texture_format format() const noexcept;

private:
    struct swapchain_object_implementation;
    std::shared_ptr<swapchain_object_implementation> _implementation;
};

}