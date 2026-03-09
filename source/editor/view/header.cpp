#include <imgui.h>

#include <editor/asset/image.hpp>
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

            if (ImGui::MenuItem("Create asset")) {
                // TODO
            }

            if (ImGui::MenuItem("Import asset")) {
                // TODO
                const std::optional<std::filesystem::path> _asset_path = open_file_dialog({}, "");
                if (_asset_path) {
                    image_import _import;
                    import_image(_asset_path.value(), _import);
					current_project->images.emplace(image_id(), _import);
                }
            }
            // TODO
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