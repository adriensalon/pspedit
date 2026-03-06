

#include <pspge.h>

#include <platform/psp/framebuffer.hpp>
#include <platform/psp/vram.hpp>

namespace pspedit {
namespace {

    constexpr u16 psp_screen_width = 480;
    constexpr u16 psp_screen_height = 272;
    constexpr u16 psp_buffer_width = 512;

    [[nodiscard]] static u32 _bytes_per_pixel(const pixel_format format)
    {
        switch (format) {
        case pixel_format::rgb565:
        case pixel_format::rgba4444:
        case pixel_format::rgba5551:
            return 2;
        case pixel_format::rgba8888:
            return 4;
        default:
            return 0;
        }
    }

    [[nodiscard]] static u32 _to_vram_offset(const void* pointer) noexcept
    {
        if (!pointer) {
            return 0;
        }

        const std::uintptr_t _base = reinterpret_cast<std::uintptr_t>(sceGeEdramGetAddr());
        const std::uintptr_t _head = reinterpret_cast<std::uintptr_t>(pointer);
        return static_cast<u32>(_head - _base);
    }

}

framebuffer::framebuffer(vram_allocator& allocator, const framebuffer_descriptor& descriptor)
    : _descriptor(descriptor)
{
    if (_descriptor.width == 0 || _descriptor.height == 0 || _descriptor.buffer_width == 0) {
        return;
    }

    const u32 color_bpp = _bytes_per_pixel(_descriptor.format);
    if (color_bpp == 0) {
        return;
    }

    const u32 color_bytes = static_cast<u32>(_descriptor.buffer_width) * static_cast<u32>(_descriptor.height) * color_bpp;

    const u32 depth_bytes = static_cast<u32>(_descriptor.buffer_width) * static_cast<u32>(_descriptor.height) * 2u;

    _draw_buffer = allocator.allocate(color_bytes, 16);

    if (_descriptor.is_presentable) {
        _display_buffer = allocator.allocate(color_bytes, 16);
    }

    if (_descriptor.has_depth) {
        _depth_buffer = allocator.allocate(depth_bytes, 16);
    }

    if (!_draw_buffer) {
        _display_buffer = nullptr;
        _depth_buffer = nullptr;
        return;
    }

    if (_descriptor.is_presentable && !_display_buffer) {
        _draw_buffer = nullptr;
        _depth_buffer = nullptr;
        return;
    }

    if (_descriptor.has_depth && !_depth_buffer) {
        _draw_buffer = nullptr;
        _display_buffer = nullptr;
        return;
    }
}

framebuffer::~framebuffer()
{
}

framebuffer framebuffer::default_framebuffer(vram_allocator& allocator)
{
    framebuffer_descriptor descriptor {};
    descriptor.width = psp_screen_width;
    descriptor.height = psp_screen_height;
    descriptor.buffer_width = psp_buffer_width;
    descriptor.format = pixel_format::rgba8888;
    descriptor.is_presentable = true;
    descriptor.has_depth = true;

    return framebuffer(allocator, descriptor);
}

b32 framebuffer::is_valid() const noexcept
{
    if (!_draw_buffer) {
        return false;
    }

    if (_descriptor.is_presentable && !_display_buffer) {
        return false;
    }

    if (_descriptor.has_depth && !_depth_buffer) {
        return false;
    }

    return true;
}

const framebuffer_descriptor& framebuffer::descriptor() const noexcept
{
    return _descriptor;
}

u32 framebuffer::draw_buffer() const noexcept
{
    return _to_vram_offset(_draw_buffer);
}

u32 framebuffer::display_buffer() const noexcept
{
    return _to_vram_offset(_display_buffer);
}

u32 framebuffer::depth_buffer() const noexcept
{
    return _to_vram_offset(_depth_buffer);
}

}