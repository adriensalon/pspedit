#include <imgui.h>

#include <core/project.hpp>
#include <view/3dview.hpp>
#include <view/browser.hpp>
#include <view/console.hpp>
#include <view/header.hpp>
#include <view/inspector.hpp>
#include <view/view.hpp>

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
    draw_header();
    draw_console();
    draw_3dview();
    draw_inspector();
    draw_browser();
}