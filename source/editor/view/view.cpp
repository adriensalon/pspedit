#include <imgui.h>
#include <imgui_internal.h>

#include <editor/core/project.hpp>
#include <editor/view/3dview.hpp>
#include <editor/view/browser.hpp>
#include <editor/view/console.hpp>
#include <editor/view/header.hpp>
#include <editor/view/inspector.hpp>
#include <editor/view/view.hpp>

namespace pspedit {
namespace {

    void _build_default_docking_layout()
    {
        ImGuiWindowFlags _host_window_flags = ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBackground;

        ImGuiViewport* _viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(_viewport->WorkPos);
        ImGui::SetNextWindowSize(_viewport->WorkSize);
        ImGui::SetNextWindowViewport(_viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::Begin("MainDockSpaceHost", nullptr, _host_window_flags);
        ImGui::PopStyleVar(3);
        ImGuiID _dockspace_id = ImGui::GetID("MainDockSpace");

        static bool _is_first_time = true;
        if (_is_first_time) {
            _is_first_time = false;
            ImGuiID _dock_main_id = _dockspace_id;
            ImGuiID _dock_left_id = 0;
            ImGuiID _dock_right_id = 0;
            ImGuiID _dock_bottom_id = 0;

            ImGui::DockBuilderRemoveNode(_dockspace_id);
            ImGui::DockBuilderAddNode(_dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(_dockspace_id, _viewport->WorkSize);
            ImGui::DockBuilderSplitNode(_dock_main_id, ImGuiDir_Left, 0.20f, &_dock_left_id, &_dock_main_id);
            ImGui::DockBuilderSplitNode(_dock_main_id, ImGuiDir_Right, 0.25f, &_dock_right_id, &_dock_main_id);
            ImGui::DockBuilderSplitNode(_dock_main_id, ImGuiDir_Down, 0.30f, &_dock_bottom_id, &_dock_main_id);
            ImGui::DockBuilderDockWindow("Browser", _dock_left_id);
            ImGui::DockBuilderDockWindow("Inspector", _dock_right_id);
            ImGui::DockBuilderDockWindow("Console", _dock_bottom_id);
            ImGui::DockBuilderDockWindow("3D View", _dock_main_id);
            ImGui::DockBuilderFinish(_dockspace_id);
        }

        ImGui::DockSpace(_dockspace_id);
        ImGui::End();
    }

}

std::string window_title()
{
    std::string _title = "PSPEdit";
    if (current_project) {
        _title += " [" + current_project->directory.string() + "]";
    } else {
        _title += " [no open project]";
    }
    return _title;
}

void draw_view()
{
    _build_default_docking_layout();
    draw_header();
    draw_console();
    draw_3dview();
    draw_inspector();
    draw_browser();
}

}