#pragma once

#include <common/core/scalar.hpp>

namespace pspedit {

using entity_id = u32;

template <typename Tag>
struct asset_id {
    u32 value = 0;

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return value != 0;
    }

    // friend bool operator==(asset_id, asset_id) = default;
};

struct image_tag { };
struct mesh_tag { };
struct material_tag { };
struct package_tag { };
struct model_tag { };
struct transform_tag { };
struct scene_tag { };

using image_id = asset_id<image_tag>;
using mesh_id = asset_id<mesh_tag>;
using material_id = asset_id<material_tag>;
using package_id = asset_id<package_tag>;
using model_id = asset_id<model_tag>;
using transform_id = asset_id<transform_tag>;
using scene_id = asset_id<scene_tag>;

template <typename Archive>
void serialize(Archive& archive, image_id& id);

template <typename Archive>
void serialize(Archive& archive, mesh_id& id);

template <typename Archive>
void serialize(Archive& archive, material_id& id);

template <typename Archive>
void serialize(Archive& archive, model_id& id);

template <typename Archive>
void serialize(Archive& archive, transform_id& id);

}