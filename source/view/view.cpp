#include <imgui.h>

#include <view/view.hpp>
#include <view/console.hpp>
#include <view/header.hpp>
#include <view/3dview.hpp>
#include <view/inspector.hpp>
#include <view/browser.hpp>

std::string window_title()
{
    return "PSPEdit (no project loaded)";
}

void draw_view()
{
    draw_header();
    draw_console();
    draw_3dview();
    draw_inspector();
    draw_browser();
}