#pragma once

#include <vector>

#include <runtime/asset/material.hpp>
#include <runtime/asset/mesh.hpp>

namespace pspedit {

struct runtime_model_component {
    shared_mesh mesh = {};
    shared_material material = {};
};

struct shared_model {
    [[nodiscard]] inline bool has_value() const { return _asset_runtime != nullptr; }
    [[nodiscard]] inline runtime_model_component* value() const { return _asset_runtime; }

private:
    runtime_model_component* _asset_runtime = nullptr;
    // later we add generation u32 for debug builds only
    friend struct content_system;
};

}