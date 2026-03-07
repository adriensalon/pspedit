#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#include <editor/core/log.hpp>
#include <editor/core/project.hpp>
#include <editor/import/stbimage.hpp>

namespace pspedit {
namespace {

    constexpr int _desired_channels = 4;
}

void import_stbimage(const std::filesystem::path& stbimage_path)
{
    if (!current_project) {
        log_error("Import", "Failed to import no project loaded");
        return;
    }

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

	image_header _header;
	_header.magic = 0;

	std::vector<u8> _pixels;
    const u32 _bytes_per_pixel = _desired_channels;
    const std::size_t _byte_count = static_cast<std::size_t>(_source_width) * static_cast<std::size_t>(_source_height) * _bytes_per_pixel;
    _header.texture.stride = static_cast<u32>(_source_width) * _bytes_per_pixel;
    _header.texture.format = pixel_format::rgba8888;
    _header.texture.filter_min = texture_filter::nearest;
    _header.texture.filter_mag = texture_filter::nearest;
    _header.texture.swizzle = 0;
    _header.texture.width = static_cast<u16>(_source_width);
    _header.texture.height = static_cast<u16>(_source_height);
    _pixels.resize(_byte_count);
    std::memcpy(_pixels.data(), _pixels_data_ptr, _byte_count);
    stbi_image_free(_pixels_data_ptr);

    // object_database_entry<image_object>& _image_entry = current_project->images.entries.emplace_back();
    // _image_entry.editor_name = stbimage_path.filename().replace_extension().string();
    // _image_entry.import_path = stbimage_path;
    // _image_entry.content_entry.id = { 4444 }; // TODO GENERATE
    // _image_entry.content_entry.path = current_project->directory / (std::to_string(_image_entry.content_entry.id.value) + ".asset");
    // _image_entry.save_object(_image);
}
}
