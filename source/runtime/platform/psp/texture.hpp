#pragma once

#include <memory>

#include <core/image.hpp>

namespace pspedit {

enum struct psp_texture_storage {
    ram,
    vram
};

struct psp_texture_descriptor {
    psp_texture_storage storage;
    u32 width;
    u32 height;
    u32 stride_width;
    texture_internal_format format;
    texture_filter min_filter;
    texture_filter mag_filter;
    u8 swizzle;
};

struct psp_texture_object {
    psp_texture_object(const psp_texture_descriptor& descriptor);
    psp_texture_object(const psp_texture_object& other) = delete;
    psp_texture_object& operator=(const psp_texture_object& other) = delete;
    psp_texture_object(psp_texture_object&& other);
    psp_texture_object& operator=(psp_texture_object&& other);
    ~psp_texture_object();

    void transfer(const psp_texture_storage storage);
    void bind();

private:
    struct psp_texture_implementation;
    std::shared_ptr<psp_texture_implementation> _implementation;
};

};