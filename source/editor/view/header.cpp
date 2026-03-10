#include <fstream>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <editor/core/assimp.hpp>
#include <editor/core/dialog.hpp>
#include <editor/core/project.hpp>
#include <editor/core/stbimage.hpp>
#include <editor/view/header.hpp>
#include <editor/view/settings.hpp>

namespace pspedit {


void draw_header()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem("Open directory...")) {
                const std::optional<std::filesystem::path> _project_directory = pick_directory_dialog("");
                if (_project_directory) {
                    open_directory(_project_directory.value());
                }
            }
            if (ImGui::MenuItem("Save all", "Ctrl+S")) {
                // TODO
            }
            if (ImGui::MenuItem("Edit settings")) {
				open_settings();
            }
            ImGui::EndMenu();
        }

        if (!current_project) {
            ImGui::BeginDisabled();
        }

        if (ImGui::BeginMenu("Asset")) {

            if (ImGui::MenuItem("Import with stbimage")) {
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    import_stbimage(_asset_path.value());
                }
            }

            if (ImGui::MenuItem("Import with assimp")) {
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    import_assimp(_asset_path.value());
                }
            }

            if (ImGui::MenuItem("Add material")) {
                project_asset<material_asset> _import = {};
                _import.editor_name = "mymaterial";
                current_project->materials.emplace(material_id(), _import); // TODO create id
                // save_asset(current_project->directory / "install/assets/material.bin", _import.asset); // TODO bake path
            }

            if (ImGui::MenuItem("Add scene")) {
                project_asset<scene_asset> _asset = {};
                _asset.editor_name = "myscene";
                current_project->scenes.emplace(scene_id(), _asset); // TODO create id
                // save_asset(current_project->directory / "install/assets/material.bin", _import.asset); // TODO bake path
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Entity")) {

            if (!current_project->selected_scene) {
                ImGui::BeginDisabled();
            }

            const scene_id _selected_scene_id = current_project->selected_scene.value();
            project_asset<scene_asset>& _selected_scene = current_project->scenes[_selected_scene_id];
            if (ImGui::MenuItem("Add entity")) {
                _selected_scene.asset.entities.emplace_back(scene_entity());
            }

            if (!current_project->selected_entity) {
                ImGui::BeginDisabled();
            }
            if (ImGui::MenuItem("Add model")) {
                model_asset _asset = {};
                model_id _id = {}; // TODOOOOOOOO
                current_project->models[_id] = _asset;
                _selected_scene.asset.entities[current_project->selected_entity.value()].model = _id;
            }
            if (ImGui::MenuItem("Add transform")) {
                transform_asset _asset = {};
                transform_id _id = {}; // TODOOOOOOOO
                current_project->transforms[_id] = _asset;
                _selected_scene.asset.entities[current_project->selected_entity.value()].transform = _id;
            }
            if (!current_project->selected_entity) {
                ImGui::EndDisabled();
            }

            if (!current_project->selected_scene) {
                ImGui::EndDisabled();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Build")) {
            if (ImGui::MenuItem("Build and run (PPSSPP)", "F5")) {
                build_and_run();
            }
            ImGui::EndMenu();
        }

        if (!current_project) {
            ImGui::EndDisabled();
        }

        ImGui::EndMainMenuBar();
    }
}
}