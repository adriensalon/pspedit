#pragma once

#include <core/project.hpp>

void import_dwarf(const std::filesystem::path& dwarf_path, object_database<pspedit::script_object>& scripts);

