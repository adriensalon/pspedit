#pragma once

namespace pspedit {

enum struct archive_type {
    binary,
    json
};

template <archive_type Type>
constexpr inline const char* archive_extension()
{
    if constexpr (Type == archive_type::binary) {
        return ".bin";
    } else if constexpr (Type == archive_type::json) {
        return ".json";
    }
}

}