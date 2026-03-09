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
            for (const std::pair<const image_id, project_imported_asset<image_asset>>& _entry : current_project->images) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                const bool _is_selected = current_project->selected_image && (current_project->selected_image.value() == _entry.first);
                if (ImGui::Selectable(_name.c_str(), _is_selected)) {
                    current_project->selected_image = _entry.first;
                    current_project->selected_mesh = std::nullopt;
                    current_project->selected_material = std::nullopt;
                    current_project->selected_scene = std::nullopt;
                    current_project->selected_entity = std::nullopt;
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Mesh")) {
            for (const std::pair<const mesh_id, project_imported_asset<mesh_asset>>& _entry : current_project->meshes) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                const bool _is_selected = current_project->selected_mesh && (current_project->selected_mesh.value() == _entry.first);
                if (ImGui::Selectable(_name.c_str(), _is_selected)) {
                    current_project->selected_image = std::nullopt;
                    current_project->selected_mesh = _entry.first;
                    current_project->selected_material = std::nullopt;
                    current_project->selected_scene = std::nullopt;
                    current_project->selected_entity = std::nullopt;
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
                    current_project->selected_mesh = std::nullopt;
                    current_project->selected_material = _entry.first;
                    current_project->selected_scene = std::nullopt;
                    current_project->selected_entity = std::nullopt;
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scene")) {
            for (const std::pair<const scene_id, project_asset<scene_asset>>& _entry : current_project->scenes) {
                const std::string _name = _entry.second.editor_name;
                if (!_filter.PassFilter(_name.c_str())) {
                    continue;
                }
                const bool _is_selected = current_project->selected_scene && (current_project->selected_scene.value() == _entry.first);
                if (ImGui::Selectable(_name.c_str(), _is_selected)) {
                    current_project->selected_image = std::nullopt;
                    current_project->selected_mesh = std::nullopt;
                    current_project->selected_material = std::nullopt;
                    current_project->selected_scene = _entry.first;
                    current_project->selected_entity = std::nullopt;
                }
            }
            ImGui::TreePop();
        }
    }

    static void _draw_entity_browser()
    {
        if (!current_project || !current_project->selected_scene) {
            return;
        }

        const scene_id _selected_scene_index = current_project->selected_scene.value();
        project_asset<scene_asset>& _selected_scene = current_project->scenes[_selected_scene_index];

        u32 _entity_index = 0;
        for (const scene_entity& _entity : _selected_scene.asset.entities) {
            const std::string _entity_name = "Entity #" + std::to_string(_entity_index);
            const bool _is_selected = current_project->selected_entity && (current_project->selected_entity.value() == _entity_index);
            if (ImGui::Selectable(_entity_name.c_str(), _is_selected)) {
                current_project->selected_entity = _entity_index;
            }
            _entity_index++;
        }
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
        if (ImGui::BeginTabItem("Entities")) {
            _draw_entity_browser();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
}