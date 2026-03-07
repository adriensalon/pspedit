#include <vector>

#include <imgui.h>

#include <editor/view/3dview.hpp>

namespace {

static void compute_center_crop_uvs(const float texture_aspect, const float view_width, const float view_height, ImVec2& uv0, ImVec2& uv1)
{
    uv0 = ImVec2(0.f, 0.f);
    uv1 = ImVec2(1.f, 1.f);

    if (view_width <= 0.f || view_height <= 0.f) {
        return;
    }

    float _view_aspect = view_width / view_height;
    if (_view_aspect > texture_aspect) {
        const float _scale = texture_aspect / _view_aspect;
        const float _pad = (1.f - _scale) * 0.5f;
        uv0.y = _pad;
        uv1.y = 1.f - _pad;
    } else if (_view_aspect < texture_aspect) {
        const float _scale = _view_aspect / texture_aspect;
        const float _pad = (1.f - _scale) * 0.5f;
        uv0.x = _pad;
        uv1.x = 1.f - _pad;
    }
}

static constexpr std::size_t _texture_width = 1600;
static constexpr std::size_t _texture_height = 900;
static std::size_t _texture_id = 0;

}

void draw_3dview()
{
    ImGui::Begin("3D View");

    const ImVec2 _available_size = ImGui::GetContentRegionAvail();
    const float _texture_aspect = static_cast<float>(_texture_width) / static_cast<float>(_texture_height);
    ImVec2 _uv0, _uv1;
    compute_center_crop_uvs(_texture_aspect, _available_size.x, _available_size.y, _uv0, _uv1);
    ImGui::Image(_texture_id, _available_size, _uv0, _uv1);
    
    ImGui::End();
}