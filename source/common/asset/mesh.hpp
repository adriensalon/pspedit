#pragma once

#include <filesystem>
#include <vector>

#include <common/core/buffer.hpp>

namespace pspedit {

struct mesh_asset {
    u16 version = 1;
    vertex_buffer_descriptor vertex_buffer = {};
    std::vector<u8> vertices = {};
    index_buffer_descriptor index_buffer = {};
    std::vector<u16> indices = {};
};

template <typename Archive>
void serialize(Archive& archive, mesh_asset& header);

bool load_asset(const std::filesystem::path& archive_path, mesh_asset& asset);
bool save_asset(const std::filesystem::path& archive_path, const mesh_asset& asset);

}
