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

namespace pspedit {

enum struct project_run_target {
    ppsspp_emulator,
    psp_usblink
};

template <typename Object>
struct object_database_entry {
    content_database_entry content_entry = {};
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
    editor_project(const std::filesystem::path& project_directory);
    editor_project(const editor_project& other) = delete;
    editor_project& operator=(const editor_project& other) = delete;
    editor_project(editor_project&& other) = default;
    editor_project& operator=(editor_project&& other) = default;
    
    object_database<image_object> images = {};
    object_database<material_object> materials = {};
    object_database<transform_object> transforms = {};
    object_database<scene_object> scenes = {};
    object_database<script_object> scripts = {};

    void load_from_project_file();
    void save_to_project_file();
    void emplace_image(const image_object& image, const std::string& editor_name, const std::optional<std::filesystem::path>& import_path);
    void erase_image(const image_object& image, const std::string& editor_name, const std::optional<std::filesystem::path>& import_path);
    void reimport_from_dwarf_file();
    void reload_assets();
    [[nodiscard]] bool is_project_buildable();
    void build_and_install_project();
    void run_project(const project_run_target target);

private:
    std::filesystem::path _project_directory;
    std::filesystem::path _install_directory;
    std::filesystem::path _cache_directory;
    std::filesystem::path _runtime_directory;
    std::filesystem::path _generated_directory;
    std::filesystem::path _reflect_directory;
    std::filesystem::path _reflect_dwarf_path;
    std::filesystem::path _build_directory;
};

}