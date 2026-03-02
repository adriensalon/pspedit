#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include <pspedit/core/content.hpp>
#include <pspedit/core/image.hpp>
#include <pspedit/core/material.hpp>
#include <pspedit/core/mesh.hpp>
#include <pspedit/core/scene.hpp>
#include <pspedit/core/script.hpp>
#include <pspedit/core/transform.hpp>

enum struct project_run_target {
    ppsspp_emulator,
    psp_usblink
};

template <typename Object>
struct object_database_entry {
    pspedit::content_database_entry content_entry = {};
    std::optional<std::filesystem::path> import_path = std::nullopt;
    std::optional<std::string> editor_name = std::nullopt;

    [[nodiscard]] Object load_object() const;
    void save_object(const Object& object) const;
};

template <typename Object>
struct object_database {
    std::optional<Object> default_object = std::nullopt;
    std::vector<object_database_entry<Object>> entries = {};
};

struct editor_project {
    object_database<pspedit::image_object> images = {};
    object_database<pspedit::material_object> materials = {};
    object_database<pspedit::transform_object> transforms = {};
    object_database<pspedit::scene_object> scenes = {};
    object_database<pspedit::script_object> scripts = {};
    std::filesystem::path directory = {};
};

inline std::optional<editor_project> current_project = std::nullopt;

void open_directory(const std::filesystem::path& project_directory);
void save_all();
void build_and_run();

void emplace_image(const pspedit::image_object& image, const std::string& editor_name, const std::optional<std::filesystem::path>& import_path);
void erase_image(const pspedit::image_object& image, const std::string& editor_name, const std::optional<std::filesystem::path>& import_path);
void reimport_from_dwarf_file();
void reload_assets();
[[nodiscard]] bool is_project_buildable();
void run_project(const project_run_target target);
