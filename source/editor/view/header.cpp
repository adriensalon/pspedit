#include <imgui.h>

#include <editor/asset/image.hpp>
#include <editor/asset/mesh.hpp>
#include <editor/core/dialog.hpp>
#include <editor/core/project.hpp>
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

            if (ImGui::MenuItem("Import image")) {
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    image_import _import;
                    import_image(_asset_path.value(), _import);
					current_project->images.emplace(image_id(), _import); // TODO create id
					save_asset(current_project->directory / "install/assets/image.bin", _import.image); // TODO bake path
                }
            }

            if (ImGui::MenuItem("Import mesh")) {
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    mesh_import _import;
                    import_mesh(_asset_path.value(), _import);
					current_project->meshes.emplace(mesh_id(), _import); // TODO create id
					save_asset(current_project->directory / "install/assets/mesh.bin", _import.asset); // TODO bake path
                }
            }

            if (ImGui::MenuItem("Create material")) {		
				material_import _import = {};
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