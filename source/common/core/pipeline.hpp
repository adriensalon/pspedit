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

struct blend_state {
    b32 enabled = false;
    blend_operation operation = blend_operation::add;
    blend_factor source = blend_factor::source_alpha;
    blend_factor destination = blend_factor::one_minus_source_alpha;
};

struct depth_state {
    b32 test_enabled = true;
    b32 write_enabled = true;
    compare_operation operation = compare_operation::less_equal;
};

struct alphatest_state {
    b32 enabled = false;
    u8 reference = 0;
    compare_operation operation = compare_operation::greater;
};

struct pipeline_descriptor {
    cull_mode cull = cull_mode::back;
    depth_state depth = {};
    blend_state blend = {};
    alphatest_state alpha_test = {};
    b32 shade_smooth = true;
    b32 dither = false;
};

}