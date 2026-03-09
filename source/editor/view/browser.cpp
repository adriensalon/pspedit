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
            for (const std::pair<const image_id, project_asset<image_asset>>& _entry : current_project->images) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                const bool _is_selected = current_project->selected_image && (current_project->selected_image.value() == _entry.first);
                if (ImGui::Selectable(_name.c_str(), _is_selected)) {
                    current_project->selected_image = _entry.first;
                    current_project->selected_mesh = std::nullopt;
                    current_project->selected_material = std::nullopt;
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Mesh")) {
			for (const std::pair<const mesh_id, project_asset<mesh_asset>>& _entry : current_project->meshes) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                const bool _is_selected = current_project->selected_mesh && (current_project->selected_mesh.value() == _entry.first);
                if (ImGui::Selectable(_name.c_str(), _is_selected)) {
                    current_project->selected_image = std::nullopt;
                    current_project->selected_mesh = _entry.first;
                    current_project->selected_material = std::nullopt;
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Material")) {
			for (const std::pair<const material_id, project_asset<material_asset>>& _entry : current_project->materials) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                const bool _is_selected = current_project->selected_material && (current_project->selected_material.value() == _entry.first);
                if (ImGui::Selectable(_name.c_str(), _is_selected)) {
                    current_project->selected_image = std::nullopt;
                    current_project->selected_material = std::nullopt;
                    current_project->selected_material = _entry.first;
                }
            }
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