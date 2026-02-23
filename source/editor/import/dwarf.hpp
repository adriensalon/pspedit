#pragma once

#include <editor/core/project.hpp>

namespace pspedit {

void import_dwarf(const std::filesystem::path& dwarf_path, object_database<script_object>& scripts);

}
