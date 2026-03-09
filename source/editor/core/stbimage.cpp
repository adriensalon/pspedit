#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#include <editor/core/log.hpp>
#include <editor/core/project.hpp>

namespace pspedit {
namespace {

    constexpr int _desired_channels = 4;
}

void import_stbimage(const std::filesystem::path& import_path)
{
    stbi_set_flip_vertically_on_load(0);
    int _source_width = 0, _source_height = 0, _source_channels = 0;
    stbi_uc* _pixels_data_ptr = stbi_load(import_path.string().c_str(), &_source_width, &_source_height, &_source_channels, _desired_channels);

    if (!_pixels_data_ptr) {
        const char* _reason = stbi_failure_reason();
        log_error("Import", "Failed to load " + import_path.string() + " with " + (_reason ? _reason : "unknown error"));
        return;
    }

    if (_source_width <= 0 || _source_height <= 0 || _source_width > 65535 || _source_height > 65535) {
        stbi_image_free(_pixels_data_ptr);
        log_error("Import", "Unsupported image dimensions from " + import_path.string() + " with " + std::to_string(_source_width) + "x" + std::to_string(_source_height));
        return;
    }

    const u32 _bytes_per_pixel = _desired_channels;
    const std::size_t _byte_count = static_cast<std::size_t>(_source_width) * static_cast<std::size_t>(_source_height) * _bytes_per_pixel;

	project_imported_asset<image_asset> _import = {};
	_import.import_path = import_path;
	_import.editor_name = import_path.filename().replace_extension().string();
	_import.asset.version = 1;
    _import.asset.texture.stride = static_cast<u32>(_source_width) * _bytes_per_pixel;
    _import.asset.texture.format = pixel_format::rgba8888; // TODO
    _import.asset.texture.filter_min = texture_filter::nearest; // TODO
    _import.asset.texture.filter_mag = texture_filter::nearest; // TODO
    _import.asset.texture.swizzle = 0;
    _import.asset.texture.width = static_cast<u16>(_source_width);
    _import.asset.texture.height = static_cast<u16>(_source_height);
    _import.asset.pixels.resize(_byte_count);
    std::memcpy(_import.asset.pixels.data(), _pixels_data_ptr, _byte_count);
    stbi_image_free(_pixels_data_ptr);
	current_project->images.emplace(image_id(), std::move(_import));
	// TODO generate new id
}

}
