#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#include <editor/core/log.hpp>
#include <editor/asset/image.hpp>

namespace pspedit {
namespace {

    constexpr int _desired_channels = 4;
}

bool import_image(const std::filesystem::path& import_path, image_import& import)
{
    stbi_set_flip_vertically_on_load(0);
    int _source_width = 0, _source_height = 0, _source_channels = 0;
    stbi_uc* _pixels_data_ptr = stbi_load(import_path.string().c_str(), &_source_width, &_source_height, &_source_channels, _desired_channels);

    if (!_pixels_data_ptr) {
        const char* _reason = stbi_failure_reason();
        log_error("Import", "Failed to load " + import_path.string() + " with " + (_reason ? _reason : "unknown error"));
        return false;
    }

    if (_source_width <= 0 || _source_height <= 0 || _source_width > 65535 || _source_height > 65535) {
        stbi_image_free(_pixels_data_ptr);
        log_error("Import", "Unsupported image dimensions from " + import_path.string() + " with " + std::to_string(_source_width) + "x" + std::to_string(_source_height));
        return false;
    }

    const u32 _bytes_per_pixel = _desired_channels;
    const std::size_t _byte_count = static_cast<std::size_t>(_source_width) * static_cast<std::size_t>(_source_height) * _bytes_per_pixel;

	import.import_path = import_path;
	import.image.version = 1;
    import.image.texture.stride = static_cast<u32>(_source_width) * _bytes_per_pixel;
    import.image.texture.format = pixel_format::rgba8888; // TODO
    import.image.texture.filter_min = texture_filter::nearest; // TODO
    import.image.texture.filter_mag = texture_filter::nearest; // TODO
    import.image.texture.swizzle = 0;
    import.image.texture.width = static_cast<u16>(_source_width);
    import.image.texture.height = static_cast<u16>(_source_height);
    import.image.pixels.resize(_byte_count);
    std::memcpy(import.image.pixels.data(), _pixels_data_ptr, _byte_count);
    stbi_image_free(_pixels_data_ptr);

	return true;
}
}
