#pragma once

#include <optional>
#include <vector>

#include <common/core/scalar.hpp>

namespace pspedit {

struct runtime_transform_component;

struct shared_transform {
    [[nodiscard]] inline bool has_value() const { return _asset_runtime != nullptr; }
    [[nodiscard]] inline runtime_transform_component* value() const { return _asset_runtime; }

private:
    runtime_transform_component* _asset_runtime = nullptr;
    // later we add generation u32 for debug builds only
    friend struct content_system;
};

struct runtime_transform_component {
    f32x4x4 trs = {}; // TODO identity
    shared_transform parent = {};
    std::vector<shared_transform> children = {};
};

}