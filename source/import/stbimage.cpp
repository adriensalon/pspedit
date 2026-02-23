#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#include <core/log.hpp>
#include <import/stbimage.hpp>

namespace {

constexpr int _desired_channels = 4;
}

void import_stbimage(const std::filesystem::path& stbimage_path, editor_project& project)
{
    stbi_set_flip_vertically_on_load(0);
    int _source_width = 0, _source_height = 0, _source_channels = 0;
    stbi_uc* _pixels_data_ptr = stbi_load(stbimage_path.string().c_str(), &_source_width, &_source_height, &_source_channels, _desired_channels);

    if (!_pixels_data_ptr) {
        const char* _reason = stbi_failure_reason();
        log_error("Import", "Failed to load " + stbimage_path.string() + " with " + (_reason ? _reason : "unknown error"));
        return;
    }

    if (_source_width <= 0 || _source_height <= 0 || _source_width > 65535 || _source_height > 65535) {
        stbi_image_free(_pixels_data_ptr);
        log_error("Import", "Unsupported image dimensions from " + stbimage_path.string() + " with " + std::to_string(_source_width) + "x" + std::to_string(_source_height));
        return;
    }

    pspedit::image_object _image = project.images.default_object ? project.images.default_object.value() : pspedit::image_object(); // TODO HANDLE DEFAULTS
    const pspedit::u32 _bytes_per_pixel = _desired_channels;
    const std::size_t _byte_count = static_cast<std::size_t>(_source_width) * static_cast<std::size_t>(_source_height) * _bytes_per_pixel;
    _image.stride_width = static_cast<pspedit::u32>(_source_width) * _bytes_per_pixel;
    _image.format = pspedit::texture_internal_format::rgba8888;
    _image.min_filter = pspedit::texture_filter::nearest;
    _image.mag_filter = pspedit::texture_filter::nearest;
    _image.swizzle = 0;
    _image.width = static_cast<pspedit::u16>(_source_width);
    _image.height = static_cast<pspedit::u16>(_source_height);
    _image.pixels.resize(_byte_count);
    std::memcpy(_image.pixels.data(), _pixels_data_ptr, _byte_count);
    stbi_image_free(_pixels_data_ptr);

    const std::string _editor_name = stbimage_path.filename().replace_extension().string();
    // project.emplace_image(_image, _editor_name, stbimage_path);
}
