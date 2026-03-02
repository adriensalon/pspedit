#pragma once

#include <variant>
#include <vector>

#include <core/id.hpp>
#include <core/image.hpp>
#include <core/material.hpp>
#include <core/mesh.hpp>
#include <core/scalar.hpp>

namespace pspedit {

struct script_object_field {
    u32 field_id;
    u32 value_type_id;
    std::variant<
        f32,
        // TODO ETC...
        image_object_reference,
        material_object_reference>
        serialized;
};

struct script_object {
    u32 script_id;
    std::vector<script_object_field> fields;
    bool has_create;
    bool has_update;
    bool has_draw;
    bool has_destroy;
};

struct script_object_reference {
    object_id id;
};

// runtime_script

template <typename Script>
struct shared_script {
    runtime_id id = null_runtime_id;
};

}