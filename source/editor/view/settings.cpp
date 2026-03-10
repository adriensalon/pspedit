#include <filesystem>
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/string.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <editor/core/project.hpp>
#include <editor/view/settings.hpp>

namespace cereal {

template <class Archive>
void save(Archive& archive, const std::filesystem::path& value)
{
    archive(value.string());
}

template <class Archive>
void load(Archive& archive, std::filesystem::path& value)
{
    std::string _temp;
    archive(_temp);
    value = _temp;
}

}

namespace pspedit {
namespace {

    static bool _is_settings_drawn = false;

}

void open_settings()
{
    _is_settings_drawn = true;
}

void draw_settings()
{
    if (_is_settings_drawn) {
        ImGui::OpenPopup("Settings");
        _is_settings_drawn = false;
    }
    const float _modal_width = 400.f;
    const ImVec2 _viewport_center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(_viewport_center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(_modal_width, 0.f), ImGuiCond_Always);
    ImGui::SetNextWindowSizeConstraints(ImVec2(_modal_width, 0.f), ImVec2(_modal_width, FLT_MAX));
    if (ImGui::BeginPopupModal("Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

        std::string _temp = current_project->custom_pspedit_dir ? current_project->custom_pspedit_dir.value().string() : "";
        if (ImGui::InputText("Custom pspedit dir", &_temp)) {
            const std::filesystem::path _dir(_temp);
            if (std::filesystem::exists(_dir)) {
                current_project->custom_pspedit_dir = _dir;
                const std::filesystem::path _settings_path = current_project->directory / "settings.psp.json";
                std::ofstream _fstream(_settings_path);
                cereal::JSONOutputArchive _archive(_fstream);
                _archive(cereal::make_nvp("name", current_project->name));
                // _archive(cereal::make_nvp("cover_image", current_project->cover_image));
                _archive(cereal::make_nvp("heap_memory", current_project->heap_memory));
                _archive(cereal::make_nvp("custom_pspedit_dir", current_project->custom_pspedit_dir));
            }
        }

		if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
		
        ImGui::EndPopup();
    }
}

}