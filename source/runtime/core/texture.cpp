#include <cstring>

#include <pspgu.h>

#include <runtime/core/swapchain.hpp>
#include <runtime/core/texture.hpp>
#include <runtime/core/vram.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] int _to_gu_psm(const pixel_format format)
    {
        switch (format) {
        case pixel_format::rgb565:
            return GU_PSM_5650;
        case pixel_format::rgba4444:
            return GU_PSM_4444;
        case pixel_format::rgba5551:
            return GU_PSM_5551;
        case pixel_format::rgba8888:
            return GU_PSM_8888;
        case pixel_format::r8:
            return GU_PSM_T8;
        case pixel_format::r16:
            return GU_PSM_T16;
        case pixel_format::r32:
            return GU_PSM_T32;
        default:
            return GU_PSM_8888;
        }
    }

    [[nodiscard]] int _to_gu_filter(const texture_filter filter)
    {
        return (filter == texture_filter::linear) ? GU_LINEAR : GU_NEAREST;
    }

    [[nodiscard]] int _to_gu_wrap(const texture_wrap wrap)
    {
        return (wrap == texture_wrap::repeat) ? GU_REPEAT : GU_CLAMP;
    }

    [[nodiscard]] std::size_t _bytes_per_pixel(const pixel_format format)
    {
        switch (format) {
        case pixel_format::r8:
            return 1;
        case pixel_format::r16:
            return 2;
        case pixel_format::r32:
            return 4;
        case pixel_format::rgb565:
            return 2;
        case pixel_format::rgba4444:
            return 2;
        case pixel_format::rgba5551:
            return 2;
        case pixel_format::rgba8888:
            return 4;
        default:
            return 0;
        }
    }

    [[nodiscard]] std::size_t _byte_size(const texture_descriptor& descriptor)
    {
        const std::size_t _per_pixel = _bytes_per_pixel(descriptor.format);
        if (_per_pixel == 0 || descriptor.width == 0 || descriptor.height == 0) {
            return 0;
        }

        std::size_t _stride = descriptor.stride ? static_cast<std::size_t>(descriptor.stride) : static_cast<std::size_t>(descriptor.width);
        _stride = std::max(_stride, static_cast<std::size_t>(descriptor.width));
        return _stride * static_cast<std::size_t>(descriptor.height) * _per_pixel;
    }

}

texture::texture(vram_allocator& allocator, const texture_descriptor& descriptor)
    : _descriptor(descriptor)
{
    const std::size_t _bytes = _byte_size(_descriptor);
    if (_bytes == 0) {
        return;
    }

    _data = allocator.allocate(_bytes, 16);
}

texture::texture(vram_allocator& allocator, const texture_descriptor& descriptor, const void* pixels)
    : _descriptor(descriptor)
{
    const std::size_t _bytes = _byte_size(_descriptor);
    if (_bytes == 0 || !pixels) {
        return;
    }

    _data = allocator.allocate(_bytes, 16);
    if (!_data) {
        return;
    }

    std::memcpy(_data, pixels, _bytes);
    vram_allocator::dcache_writeback(_data, _bytes);
}

texture::~texture()
{
    // VRAM is from bump allocator -> no per texture free
}

void texture::update(vram_allocator& allocator, const texture_descriptor& descriptor, const void* pixels)
{
    if (!pixels) {
        return;
    }

    const std::size_t _old_bytes = (_data) ? _byte_size(_descriptor) : 0;
    const std::size_t _new_bytes = _byte_size(descriptor);
    if (_new_bytes == 0) {
        return;
    }

    if (!_data || _new_bytes != _old_bytes) {
        void* _new_data = allocator.allocate(_new_bytes, 16);
        if (!_new_data) {
            return;
        }
        _data = _new_data;
    }
    _descriptor = descriptor;

    std::memcpy(_data, pixels, _new_bytes);
    vram_allocator::dcache_writeback(_data, _new_bytes);
}

void texture::bind(swapchain& context, const u8 mip_level) const
{
    if (!_data) {
        return;
    }

    const int _format = _to_gu_psm(_descriptor.format);
    const int _width = static_cast<int>(_descriptor.width);
    const int _height = static_cast<int>(_descriptor.height);
    const int _stride = std::max(_width, _descriptor.stride ? static_cast<int>(_descriptor.stride) : _width);
    const int _filter_min = _to_gu_filter(_descriptor.filter_min);
    const int _filter_mag = _to_gu_filter(_descriptor.filter_mag);
    const int _wrap_u = _to_gu_wrap(_descriptor.wrap_u);
    const int _wrap_v = _to_gu_wrap(_descriptor.wrap_v);

    sceGuEnable(GU_TEXTURE_2D);
    sceGuTexMode(_format, /*mipmaps*/ 0, /*unknown*/ 0, _descriptor.swizzle ? GU_TRUE : GU_FALSE);
    sceGuTexImage(0, _width, _height, _stride, _data);
    sceGuTexFilter(_filter_min, _filter_mag);
    sceGuTexWrap(_wrap_u, _wrap_v);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
}

b32 texture::is_valid() const noexcept
{
    return static_cast<b32>(_data != nullptr);
}

const texture_descriptor& texture::descriptor() const noexcept
{
	return _descriptor;
}

void* texture::data() const noexcept
{
	return _data;
}

}