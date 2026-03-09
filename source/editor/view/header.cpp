#include <imgui.h>

#include <editor/core/assimp.hpp>
#include <editor/core/dialog.hpp>
#include <editor/core/project.hpp>
#include <editor/core/stbimage.hpp>
#include <editor/view/header.hpp>

namespace pspedit {
namespace {

}

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
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Asset")) {

            if (ImGui::MenuItem("Import with stbimage")) {
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    import_stbimage(_asset_path.value());
                }
            }

            if (ImGui::MenuItem("Import mesh")) {
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    import_assimp(_asset_path.value());
                }
            }

            if (ImGui::MenuItem("Create material")) {
                project_asset<material_asset> _import = {};
                _import.editor_name = "mymaterial";
                current_project->materials.emplace(material_id(), _import); // TODO create id
                // save_asset(current_project->directory / "install/assets/material.bin", _import.asset); // TODO bake path
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Entity")) {
            // TODO
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Build")) {
            if (ImGui::MenuItem("Build and run (PPSSPP)", "F5")) {
                build_and_run();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
}