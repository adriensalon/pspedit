#pragma once

#include <editor/core/project.hpp>

namespace pspedit {

void import_assimp(const std::filesystem::path& assimp_path, editor_project& project);

}