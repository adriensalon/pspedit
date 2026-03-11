#pragma once

#include <optional>

#include <common/core/id.hpp>
#include <runtime/core/buffer.hpp>

namespace pspedit {

struct runtime_mesh {
    std::optional<vertex_buffer> vertices = {};
    std::optional<index_buffer> indices = {};
};

struct shared_mesh {
    [[nodiscard]] inline bool has_value() const { return _asset_runtime != nullptr; }
    [[nodiscard]] inline runtime_mesh* value() const { return _asset_runtime; }

private:
	runtime_mesh* _asset_runtime = nullptr;
	// later we add generation u32 for debug builds only
	friend struct content_system;
};

}