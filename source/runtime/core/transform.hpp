#pragma once

#include <optional>
#include <vector>

#include <core/id.hpp>
#include <core/scalar.hpp>

namespace pspedit {

struct transform_object_reference {
    object_id id;
};

struct transform_object {
    mat4_f32 transform;
    std::vector<transform_object_reference> children;
};

struct shared_transform {
    runtime_id id = null_runtime_id;
};

struct runtime_transform {
    runtime_transform();
    runtime_transform(const shared_transform parent);
    runtime_transform(const vec3_f32& position);
    runtime_transform(const mat4_f32& transform_trs_packed);
    runtime_transform(const runtime_transform& other) = delete;
    runtime_transform& operator=(const runtime_transform& other) = delete;
    runtime_transform(runtime_transform&& other) = default;
    runtime_transform& operator=(runtime_transform&& other) = default;

    void use_parent(const shared_transform parent);
    void set_position_relative(const vec3_f32& position);
    void set_position_warp(const vec3_f32& position);
    void set_rotation_relative(const vec3_f32& rotation_euler_radians);
    void set_rotation_warp(const vec3_f32& rotation_euler_radians);
    void set_transform_relative(const mat4_f32& transform_trs_packed);
    void set_transform_warp(const mat4_f32& transform_trs_packed);
    [[nodiscard]] vec3_f32 get_position() const;
    [[nodiscard]] vec3_f32 get_rotation() const;
    [[nodiscard]] vec3_f32 get_right() const;
    [[nodiscard]] vec3_f32 get_up() const;
    [[nodiscard]] vec3_f32 get_forward() const;

private:
    mat4_f32 _transform;
    std::optional<shared_transform> _parent;
    std::vector<shared_transform> _children;
};

}