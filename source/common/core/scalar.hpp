#pragma once

#include <cstdint>
#include <array>

namespace pspedit {

using f32 = float;
using i32 = std::int32_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using b32 = std::int32_t;

using f32x2 = std::array<f32, 2>;
using f32x3 = std::array<f32, 3>;
using f32x4 = std::array<f32, 4>;

using u32x2 = std::array<u32, 2>;
using u32x3 = std::array<u32, 3>;
using u32x4 = std::array<u32, 4>;

using f32x2x2 = std::array<std::array<f32, 2>, 2>;
using f32x3x3 = std::array<std::array<f32, 3>, 3>;
using f32x4x4 = std::array<std::array<f32, 4>, 4>;

}