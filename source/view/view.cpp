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
    const std::optional<std::filesystem::path> _project_directory = project_directory();
    std::string _title = "PSPEdit";
    if (_project_directory) {
        _title += " [" + _project_directory.value().string() + "]";
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