#include <imgui.h>

#include <editor/core/project.hpp>
#include <editor/view/3dview.hpp>
#include <editor/view/browser.hpp>
#include <editor/view/console.hpp>
#include <editor/view/header.hpp>
#include <editor/view/inspector.hpp>
#include <editor/view/view.hpp>

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