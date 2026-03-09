#pragma once

#include <common/asset/mesh.hpp>

namespace pspedit {

struct mesh_import {
    mesh_asset asset = {};
    std::string editor_name = {};
    std::filesystem::path import_path = "";
};

void import_mesh(const std::filesystem::path& import_path, mesh_import& import);

}
