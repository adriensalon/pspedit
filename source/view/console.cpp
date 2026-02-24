#include <imgui.h>

#include <core/log.hpp>
#include <view/console.hpp>

namespace {

static ImGuiTextFilter _filter;
static bool _is_auto_scroll = true;

static void _draw_log_category(const std::vector<console_info>& infos, const ImVec4& category_color)
{
    ImGui::BeginChild("##log_category", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (const console_info& _info : infos) {
        std::string _combined;
        _combined.reserve(_info.category.size() + 2 + _info.text.size());
        _combined += _info.category;
        _combined += ": ";
        _combined += _info.text;
        if (!_filter.PassFilter(_combined.c_str())) {
            continue;
        }

        ImGui::TextUnformatted("[");
        ImGui::SameLine(0.f, 0.f);
        ImGui::PushStyleColor(ImGuiCol_Text, category_color);
        ImGui::TextUnformatted(_info.category.c_str());
        ImGui::PopStyleColor();
        ImGui::SameLine(0.f, 0.f);
        ImGui::TextUnformatted("]");
        ImGui::SameLine();
        ImGui::PushTextWrapPos(0.0f);
        ImGui::TextUnformatted(_info.text.c_str());
        ImGui::PopTextWrapPos();
    }

    if (_is_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.0f) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
}

}

void draw_console()
{
    if (!ImGui::Begin("Console")) {
        ImGui::End();
        return;
    }
    if (ImGui::Button("Clear")) {
        errors().clear();
        warnings().clear();
        messages().clear();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &_is_auto_scroll);
    ImGui::SameLine();
    _filter.Draw("Search");
    ImGui::Spacing();

    if (ImGui::BeginTabBar("##console_tabs")) {
        if (ImGui::BeginTabItem("Messages")) {
            _draw_log_category(messages(), ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Warnings")) {
            _draw_log_category(warnings(), ImVec4(1.00f, 0.75f, 0.20f, 1.0f));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Errors")) {
            _draw_log_category(errors(), ImVec4(1.00f, 0.25f, 0.25f, 1.0f));
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}