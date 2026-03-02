#pragma once

#include <core/script.hpp>
#include <core/transform.hpp>

namespace pspedit {

struct scene_object {
    std::vector<entity_id> entities;
    std::vector<script_object> scripts; // go sparse
    std::vector<transform_object> transforms; // go sparse
};

struct scene_object_reference {
    object_id id;
};

struct runtime_scene {

    [[nodiscard]] bool has_transform(const entity_id entity);
    template <typename Script>
    [[nodiscard]] bool has_script(const entity_id entity);

    [[nodiscard]] shared_transform emplace_transform(const entity_id entity);
    [[nodiscard]] shared_transform emplace_transform(const entity_id entity, const shared_transform parent);
    [[nodiscard]] shared_transform emplace_transform(const entity_id entity, const vec3_f32& position);
    [[nodiscard]] shared_transform emplace_transform(const entity_id entity, const mat4_f32& transform_trs_packed);
    template <typename Script>
    [[nodiscard]] shared_script<Script> emplace_script(const entity_id entity);
};

struct share_scene {

};

}