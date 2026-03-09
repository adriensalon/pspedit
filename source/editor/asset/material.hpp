#pragma once

#include <common/asset/material.hpp>

namespace pspedit {

struct material_import {
    material_asset asset = {};
	std::string editor_name = {};
    std::filesystem::path import_path = "";
};

}
