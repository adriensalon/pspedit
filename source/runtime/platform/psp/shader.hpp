#pragma once

#include <memory>

#include <core/material.hpp>

namespace pspedit {

struct psp_shader_descriptor {
    material_cull_format cull;
    material_depth_format depth;
    material_blend_format blend;
};

struct psp_shader_object {
    psp_shader_object(const psp_shader_descriptor& descriptor = {});
    psp_shader_object(const psp_shader_object&) = delete;
    psp_shader_object& operator=(const psp_shader_object&) = delete;
    psp_shader_object(psp_shader_object&&) = default;
    psp_shader_object& operator=(psp_shader_object&&) = default;
    ~psp_shader_object();

    // void bind(psp_command_object& command) const;

private:
    struct psp_shader_implementation;
    std::shared_ptr<psp_shader_implementation> _implementation;
};

}