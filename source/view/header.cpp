#include <imgui.h>

#include <core/dialog.hpp>
#include <core/project.hpp>
#include <view/header.hpp>

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

        if (ImGui::BeginMenu("Import")) {
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