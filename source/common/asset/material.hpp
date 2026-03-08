#pragma once

#include <filesystem>
#include <optional>

#include <common/core/id.hpp>
#include <common/core/pipeline.hpp>

namespace pspedit {

enum struct texture_function : u8 {
    modulate,
    decal,
    blend,
    replace,
    add
};

struct material_asset {
	u16 version = 1;
    pipeline_descriptor pipeline = {};
    texture_function function = texture_function::add;
    std::optional<image_id> image = std::nullopt;
    b32 is_texture_enabled = false;
    b32 is_lighting_enabled = false;
    b32 use_vertex_color = true;
    u32x4 ambient_color = { 255, 255, 255, 255 };
    u32x4 diffuse_color = { 255, 255, 255, 255 };
    u32x4 specular_color = { 0, 0, 0, 255 };
    u32x4 emissive_color = { 0, 0, 0, 255 };
    f32 shininess = 0.f;
};

template <typename Archive>
void serialize(Archive& archive, material_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, material_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const material_asset& asset);

}