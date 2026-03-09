#include <limits>
#include <type_traits>

#include <imgui.h>

#include <editor/core/project.hpp>
#include <editor/view/inspector.hpp>

namespace pspedit {
namespace {

    static const std::vector<std::string> _pixel_format_names = {
        "R4",
        "R8",
        "R16",
        "R32",
        "RGB565",
        "RGBA4444",
        "RGBA5551",
        "RGBA8888",
        "DXT1",
        "DXT3",
        "DXT5"
    };

    static const std::vector<std::string> _texture_filter_names = {
        "Nearest",
        "Linear"
    };

    static const std::vector<std::string> _texture_wrap_names = {
        "Clamp",
        "Repeat"
    };

    static const std::vector<std::string> _compare_operation_names = {
        "Never",
        "Less",
        "Less or equal",
        "Equal",
        "Greater or equal",
        "Greater",
        "Not equal",
        "Always"
    };

    static const std::vector<std::string> _cull_mode_names = {
        "None",
        "Front",
        "Back"
    };

    static const std::vector<std::string> _blend_factor_names = {
        "Zero",
        "One",
        "Source alpha",
        "One minus source alpha",
        "Destination alpha",
        "One minus destination alpha"
    };

    static const std::vector<std::string> _blend_operation_names = {
        "Add",
        "Substract",
        "Reverse substract"
    };

    template <typename Enum>
    bool _draw_inspector_enum(const std::string& label, Enum& value, const std::vector<std::string>& names)
    {
        static_assert(std::is_enum_v<Enum>, "Value type must be enum");

        int _int_value = static_cast<int>(value);
        if (_int_value < 0 || _int_value >= static_cast<int>(names.size())) {
            _int_value = 0;
        }

        bool _is_changed = false;
        const char* _preview_cstr = names[_int_value].c_str();
        if (ImGui::BeginCombo(label.c_str(), _preview_cstr)) {
            for (int _name_index = 0; _name_index < static_cast<int>(names.size()); ++_name_index) {
                bool _is_selected = (_name_index == _int_value);
                if (ImGui::Selectable(names[_name_index].c_str(), _is_selected)) {
                    value = static_cast<Enum>(_name_index);
                    _is_changed = true;
                }
                if (_is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        return _is_changed;
    }

    template <typename Integer>
    bool _draw_inspector_integer(const std::string& label, Integer& value, const Integer min = std::numeric_limits<Integer>::min(), const Integer max = std::numeric_limits<Integer>::max())
    {
        static_assert(std::is_arithmetic_v<Integer>, "Value type must be arithmetic");
        int _int_value = static_cast<int>(value);
        const int _int_min = static_cast<int>(min);
        const int _int_max = static_cast<int>(max);

        const bool _is_changed = ImGui::DragInt(label.c_str(), &_int_value, 1.f, _int_min, _int_max);
        if (_is_changed) {
            value = static_cast<Integer>(_int_value);
        }

        return _is_changed;
    }

    template <typename Boolean>
    bool _draw_inspector_boolean(const std::string& label, Boolean& value)
    {
        static_assert(std::is_arithmetic_v<Boolean>, "Value type must be arithmetic");
        bool _bool_value = value != 0;

        const bool _is_changed = ImGui::Checkbox(label.c_str(), &_bool_value);
        if (_is_changed) {
            value = static_cast<Boolean>(_bool_value);
        }

        return _is_changed;
    }

    void _draw_inspector_image()
    {
        if (current_project->selected_image) {
            project_asset<image_asset> _import = current_project->images[current_project->selected_image.value()];
            ImGui::Text("Asset version %u", _import.asset.version);
            if (_draw_inspector_integer("Width", _import.asset.texture.width)
                || _draw_inspector_integer("Height", _import.asset.texture.height)
                || _draw_inspector_integer("Stride", _import.asset.texture.stride)
                || _draw_inspector_enum("Format", _import.asset.texture.format, _pixel_format_names)
                || _draw_inspector_enum("Filter min", _import.asset.texture.filter_min, _texture_filter_names)
                || _draw_inspector_enum("Filter mag", _import.asset.texture.filter_mag, _texture_filter_names)
                || _draw_inspector_enum("Wrap U", _import.asset.texture.wrap_u, _texture_wrap_names)
                || _draw_inspector_enum("Wrap V", _import.asset.texture.wrap_v, _texture_wrap_names)) { // gpu image visualizer LATER
                save_asset(current_project->directory / "install/assets/okok.bin", _import.asset); // TODO bake path
            }
        }
    }

    void _draw_inspector_mesh()
    {
        if (current_project->selected_mesh) {
            project_asset<mesh_asset>& _import = current_project->meshes[current_project->selected_mesh.value()];
            ImGui::Text("Asset version %u", _import.asset.version);
            // if (_draw_inspector_integer("Vertex buffer stride", _import.asset.vertex_buffer.vertex.stride)
            //     || _draw_inspector_enum("Vertex buffer usage", _import.asset.vertex_buffer.usage, _pixel_format_names)
            //     || _draw_inspector_enum("Filter min", _import.asset.texture.filter_min, _texture_filter_names)
            //     || _draw_inspector_enum("Filter mag", _import.asset.texture.filter_mag, _texture_filter_names)
            //     || _draw_inspector_enum("Wrap U", _import.asset.texture.wrap_u, _texture_wrap_names)
            //     || _draw_inspector_enum("Wrap V", _import.asset.texture.wrap_v, _texture_wrap_names)) { // gpu image visualizer LATER
            //     save_asset(current_project->directory / "install/assets/okok.bin", _import.asset); // TODO bake path
            // }
        }
    }

    void _draw_inspector_material()
    {
        if (current_project->selected_material) {
            project_asset<material_asset>& _import = current_project->materials[current_project->selected_material.value()];
            ImGui::Text("Asset version %u", _import.asset.version);
            if (_draw_inspector_enum("Cull mode", _import.asset.pipeline.cull, _cull_mode_names)
                || _draw_inspector_boolean("Depth test enabled", _import.asset.pipeline.is_depth_test_enabled)
                || _draw_inspector_boolean("Depth write enabled", _import.asset.pipeline.is_depth_write_enabled)
                || _draw_inspector_enum("Depth operation", _import.asset.pipeline.depth_operation, _compare_operation_names)
            //     || _draw_inspector_enum("Filter mag", _import.asset.texture.filter_mag, _texture_filter_names)
            //     || _draw_inspector_enum("Wrap U", _import.asset.texture.wrap_u, _texture_wrap_names)
            //     || _draw_inspector_enum("Wrap V", _import.asset.texture.wrap_v, _texture_wrap_names)
			) { // gpu image visualizer LATER
                // save_asset(current_project->directory / "install/assets/okok.bin", _import.asset); // TODO bake path
            }
        }
    }

}

void draw_inspector()
{
    ImGui::Begin("Inspector");
    if (current_project) {
        _draw_inspector_image();
        _draw_inspector_mesh();
        _draw_inspector_material();
    }
    ImGui::End();
}

}