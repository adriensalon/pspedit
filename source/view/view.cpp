#include <imgui.h>

#include <view/view.hpp>
#include <view/console.hpp>
#include <view/header.hpp>

std::string window_title()
{
    return "PSPEdit (no project loaded)";
}

void draw_view()
{
    draw_header();
    draw_console();
}