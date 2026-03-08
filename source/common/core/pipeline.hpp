#pragma once

#include <common/core/scalar.hpp>

namespace pspedit {

enum struct compare_operation : u8 {
    never,
    less,
    less_equal,
    equal,
    greater_equal,
    greater,
    not_equal,
    always
};

enum struct cull_mode : u8 {
    none,
    front,
    back
};

enum struct blend_factor : u8 {
    zero,
    one,
    source_alpha,
    one_minus_source_alpha,
    destination_alpha,
    one_minus_destination_alpha
};

enum struct blend_operation : u8 {
    add,
    substract,
    reverse_substract
};

struct pipeline_descriptor {
    cull_mode cull = cull_mode::back;
    b32 is_depth_test_enabled = true;
    b32 is_depth_write_enabled = true;
    compare_operation depth_operation = compare_operation::less_equal;
    b32 is_blend_enabled = false;
    blend_operation blend = blend_operation::add;
    blend_factor blend_source = blend_factor::source_alpha;
    blend_factor blend_destination = blend_factor::one_minus_source_alpha;
    b32 is_dither_enabled = false;
    b32 is_alphatest_enabled = false;
    u8 alphatest_reference = 0;
    compare_operation alphatest_operation = compare_operation::greater;
	b32 is_smooth_shading_enabled = true;
};

template <typename Archive>
void serialize(Archive& archive, pipeline_descriptor& descriptor);

}