#pragma once

#include <common/core/scalar.hpp>

namespace pspedit {

template <typename Tag>
struct asset_id {
    u32 value = 0;

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return value != 0;
    }

    // friend bool operator==(asset_id, asset_id) = default;
};

struct image_tag {};

using image_id = asset_id<image_tag>;

template <typename Archive>
void serialize(Archive& archive, image_id& id);

}