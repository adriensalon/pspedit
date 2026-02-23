#include <imgui.h>

#include <core/project.hpp>
#include <view/header.hpp>

namespace {

}

void draw_header()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem("Open directory...")) {
                open_directory("C:/Users/adri/Desktop/pspedit_proj");
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
                // TODO
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}