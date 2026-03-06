#include <pspge.h>
#include <pspkernel.h>

#include <runtime/core/vram.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] static std::uintptr_t _align_up(const std::uintptr_t value, const std::size_t alignment)
    {
        const std::uintptr_t _mask = static_cast<std::uintptr_t>(alignment - 1);
        return (value + _mask) & ~_mask;
    }

}

vram_allocator::vram_allocator()
    : _base(reinterpret_cast<std::uint8_t*>(sceGeEdramGetAddr()))
    , _size(static_cast<std::size_t>(sceGeEdramGetSize()))
    , _head(0)
{
}

void vram_allocator::reset()
{
    _head = 0;
}

void* vram_allocator::allocate(const std::size_t bytes, const std::size_t alignment)
{
    const std::uintptr_t _current_head = reinterpret_cast<std::uintptr_t>(_base) + _head;
    const std::uintptr_t _aligned_head = _align_up(_current_head, alignment);
    const std::size_t _new_head = static_cast<std::size_t>(_aligned_head - reinterpret_cast<std::uintptr_t>(_base)) + bytes;
    if (_new_head > _size) {
        return nullptr;
    }
    _head = _new_head;
    return reinterpret_cast<void*>(_aligned_head);
}

std::size_t vram_allocator::size_bytes() const noexcept
{
    return _size;
}

std::size_t vram_allocator::used_bytes() const noexcept
{
    return _head;
}

void vram_allocator::dcache_writeback(const void* pointer, const std::size_t bytes)
{
    if (!pointer || bytes == 0) {
        return;
    }
    sceKernelDcacheWritebackRange(pointer, bytes);
}

void vram_allocator::dcache_invalidate(const void* pointer, const std::size_t bytes)
{
    if (!pointer || bytes == 0) {
        return;
    }
    sceKernelDcacheInvalidateRange(pointer, bytes);
}

}