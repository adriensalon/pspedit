#pragma once

#include <core/scalar.hpp>

namespace pspedit {

enum struct texture_internal_format {
    rgba5650,
    rgba4444,
    rgba5551,
    rgba8888
};

enum struct texture_filter {
    nearest,
    linear
};

enum struct material_face_select_format {
    back,
    front,
    front_and_back
};

enum struct material_culling_format {
    clockwise,
    counter_clockwise
};

enum struct material_depth_function_format {
    less_equal
};

enum struct material_blend_operation_format {
    add
};

enum struct material_blend_mode_format {
    source_alpha,
    one_minus_source_alpha
};

struct material_cull_format {
    bool is_enabled = false;
    material_face_select_format cull_face = material_face_select_format::back;
    material_culling_format front_face = material_culling_format::counter_clockwise;
};

struct material_depth_format {
    bool is_test_enabled = false;
    bool is_write_enabled = true;
    material_depth_function_format depth_function = material_depth_function_format::less_equal;
};

struct material_blend_format {
    bool is_enabled = false;
    material_blend_operation_format blend_operation = material_blend_operation_format::add;
    material_blend_mode_format blend_source = material_blend_mode_format::source_alpha;
    material_blend_mode_format blend_destination = material_blend_mode_format::one_minus_source_alpha;
};

}