#pragma once

#include <optional>

#include <core/graphics.hpp>
#include <core/id.hpp>
#include <core/image.hpp>
#include <platform/psp/shader.hpp>

namespace pspedit {

struct material_object {
    image_object_reference albedo;
    image_object_reference normal;
    material_cull_format cull;
    material_depth_format depth;
    material_blend_format blend;
    u32 color = 0xFFFFFFFF;
};

struct material_object_reference {
    object_id id;
};

struct runtime_material {
    runtime_material(const psp_shader_descriptor& descriptor);
    runtime_material(const runtime_material& other) = delete;
    runtime_material& operator=(const runtime_material& other) = delete;
    runtime_material(runtime_material&& other) = default;
    runtime_material& operator=(runtime_material&& other) = default;
    ~runtime_material();

    runtime_id id;
    psp_shader_object psp_shader;

    shared_image albedo;
    shared_image normal;
    u32 color = 0xFFFFFFFF;
};

}