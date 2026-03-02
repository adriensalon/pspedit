#pragma once

#include <array>

namespace pspedit {

using f32 = float;
using i32 = std::int32_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using vec2_f32 = std::array<f32, 2>;
using vec3_f32 = std::array<f32, 3>;
using vec4_f32 = std::array<f32, 4>;

using mat2_f32 = std::array<std::array<f32, 2>, 2>;
using mat3_f32 = std::array<std::array<f32, 3>, 3>;
using mat4_f32 = std::array<std::array<f32, 4>, 4>;

}