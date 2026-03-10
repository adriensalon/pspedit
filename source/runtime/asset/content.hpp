#pragma once

#include <common/asset/content.hpp>

namespace pspedit {

struct vram_allocator;

struct content {
	content(vram_allocator& allocator, const content_asset& asset);
    content(const content& other) = delete;
    content& operator=(const content& other) = delete;
    content(content&& other) noexcept = default;
    content& operator=(content&& other) noexcept = delete;
	~content();

private:
	content_asset _asset;
};

}