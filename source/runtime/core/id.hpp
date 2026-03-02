#pragma once

#include <core/scalar.hpp>

namespace pspedit {

using entity_id = u32;

struct object_id {
    u32 value = 0;

    [[nodiscard]] inline bool has_value() const
    {
        return value != 0;
    }
};

static constexpr object_id null_object_id { 0 };

struct runtime_id {
    u64 low;
    u64 high;

    [[nodiscard]] inline bool has_value() const
    {
        return (low != 0) && (high != 0);
    }
};

static constexpr runtime_id null_runtime_id { 0, 0 };

}