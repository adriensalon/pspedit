#include <cstring>

#include <platform/psp/buffer.hpp>
#include <platform/psp/vram.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] u32 _required_bytes(const vertex_buffer_descriptor& descriptor)
    {
        if (descriptor.vertex.stride == 0 || descriptor.count == 0) {
            return 0;
        }
        return static_cast<u32>(descriptor.vertex.stride) * descriptor.count;
    }

}

vertex_buffer::vertex_buffer(vram_allocator& allocator, const vertex_buffer_descriptor& descriptor)
    : _descriptor(descriptor)
{
    _bytes = _required_bytes(_descriptor);
    if (_bytes == 0) {
        return;
    }

    _data = allocator.allocate(_bytes, 16);
}

vertex_buffer::vertex_buffer(vram_allocator& allocator, const vertex_buffer_descriptor& descriptor, const void* vertices)
    : _descriptor(descriptor)
{
    _bytes = _required_bytes(_descriptor);
    if (_bytes == 0) {
        return;
    }

    _data = allocator.allocate(_bytes, 16);
    if (!_data) {
        return;
    }

    std::memcpy(_data, vertices, _bytes);
    vram_allocator::dcache_writeback(_data, _bytes);
}

vertex_buffer::~vertex_buffer()
{
    // VRAM is bump allocated -> no per buffer free
}

void vertex_buffer::update(vram_allocator& allocator, const vertex_buffer_descriptor& descriptor, const void* vertices)
{
    if (!vertices) {
        return;
    }

    const std::size_t _new_bytes = _required_bytes(descriptor);
    if (_new_bytes == 0) {
        return;
    }

    if (!_data || _new_bytes != _bytes) {
        void* _new_data = allocator.allocate(_new_bytes, 16);
        if (!_new_data) {
            return;
        }
        _data = _new_data;
        _bytes = _new_bytes;
    }
    _descriptor = descriptor;

    std::memcpy(_data, vertices, _bytes);
    vram_allocator::dcache_writeback(_data, _bytes);
}

b32 vertex_buffer::is_valid() const noexcept
{
	return static_cast<b32>(_data != nullptr);
}

u32 vertex_buffer::size_bytes() const noexcept
{
	return _bytes;
}

const vertex_buffer_descriptor& vertex_buffer::descriptor() const noexcept
{
	return _descriptor;
}

void* vertex_buffer::data() const noexcept
{
	return _data;
}

}