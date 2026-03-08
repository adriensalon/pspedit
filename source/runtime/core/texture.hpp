#pragma once

#include <common/core/texture.hpp>

namespace pspedit {

struct vram_allocator;

struct texture {
    texture(vram_allocator& allocator, const texture_descriptor& descriptor);
    texture(vram_allocator& allocator, const texture_descriptor& descriptor, const void* pixels);
    texture(const texture& other) = delete;
    texture& operator=(const texture& other) = delete;
    texture(texture&& other) noexcept = default;
    texture& operator=(texture&& other) noexcept = default;
	~texture();

	void update(vram_allocator& allocator, const texture_descriptor& descriptor, const void* pixels);
    void bind(const u8 mip_level = 0) const;
    [[nodiscard]] b32 is_valid() const noexcept;
    [[nodiscard]] const texture_descriptor& descriptor() const noexcept;
    [[nodiscard]] void* data() const noexcept;

private:
    texture_descriptor _descriptor;
    void* _data;
};

}