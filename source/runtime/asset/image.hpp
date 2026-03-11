#pragma once

#include <optional>

#include <common/core/id.hpp>
#include <runtime/core/texture.hpp>

namespace pspedit {

struct runtime_image {
    std::optional<texture> pixels = {};
};

struct shared_image {
    [[nodiscard]] inline bool has_value() const { return _asset_runtime != nullptr; }
    [[nodiscard]] inline runtime_image* value() const { return _asset_runtime; }

private:
	runtime_image* _asset_runtime = nullptr;
	// later we add generation u32 for debug builds only
	friend struct content_system;
};

}