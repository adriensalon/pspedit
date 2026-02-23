#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace pspedit {

struct dwarf_asset_reference {
    std::filesystem::path path;
    std::string target_type;
};

struct dwarf_component_reference {
    std::string name;
    std::string target_type;
};

struct dwarf_entity_reference {
    std::string name;
};

struct dwarf_scene_reference {
    std::string name;
};

struct dwarf_script {
    std::string qualified_name;
    std::vector<dwarf_asset_reference> asset_references;
    std::vector<dwarf_component_reference> component_references;
    std::vector<dwarf_entity_reference> entity_references;
    std::vector<dwarf_scene_reference> scene_references;
    bool has_on_create = false;
    bool has_on_update = false;
    bool has_on_destroy = false;
};

[[nodiscard]] std::vector<dwarf_script> dwarf_parse(const std::filesystem::path& dwarf_file);

}
