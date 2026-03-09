#include <imgui.h>

#include <editor/core/project.hpp>
#include <editor/view/browser.hpp>

namespace pspedit {
namespace {

    static ImGuiTextFilter _filter = {};

    static void _draw_assets_browser()
    {
        if (!current_project) {
            return;
        }

        if (ImGui::TreeNode("Image")) {
            for (const std::pair<const image_id, image_import>& _entry : current_project->images) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                ImGui::Selectable(_name.c_str(), false);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Material")) {
            // for (const object_database_entry<pspedit::material_object>& _entry : current_project->materials.entries) {
            //     const std::string _name = _entry.editor_name ? _entry.editor_name.value() : "untitled";
            //     if (!_filter.PassFilter(_name.c_str())) {
            //         continue;
            //     }
            //     ImGui::Selectable(_name.c_str(), false);
            // }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Mesh")) {
            ImGui::TreePop();
        }
    }

    static void _draw_current_scene_browser()
    {
    }

}

void draw_browser()
{

    if (!ImGui::Begin("Browser")) {
        ImGui::End();
        return;
    }

    _filter.Draw("Search");
    ImGui::Spacing();

    if (ImGui::BeginTabBar("##browser_tabs")) {
        if (ImGui::BeginTabItem("Assets")) {
            _draw_assets_browser();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Current scene")) {

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
}