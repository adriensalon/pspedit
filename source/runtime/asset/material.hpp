#pragma once

#include <optional>

#include <common/asset/material.hpp>
#include <common/core/id.hpp>
#include <runtime/asset/image.hpp>

namespace pspedit {

struct runtime_material {
    pipeline_descriptor pipeline = {};
    texture_function function = texture_function::add;
    std::optional<shared_image> image = std::nullopt;
    b32 is_texture_enabled = false;
    b32 is_lighting_enabled = false;
    b32 use_vertex_color = true;
    u32x4 ambient_color = { 255, 255, 255, 255 };
    u32x4 diffuse_color = { 255, 255, 255, 255 };
    u32x4 specular_color = { 0, 0, 0, 255 };
    u32x4 emissive_color = { 0, 0, 0, 255 };
    f32 shininess = 0.f;
};

struct shared_material {
    [[nodiscard]] inline bool has_value() const { return _asset_runtime != nullptr; }
    [[nodiscard]] inline runtime_material* value() const { return _asset_runtime; }

private:
    runtime_material* _asset_runtime = nullptr;
    // later we add generation u32 for debug builds only
    friend struct content_system;
};

}