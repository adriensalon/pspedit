#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <common/asset/image.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, image_header& header)
{
    archive(cereal::make_nvp("magic", header.magic));
    archive(cereal::make_nvp("version", header.version));
    // archive(cereal::make_nvp("id", header.id));
    archive(cereal::make_nvp("texture", header.texture));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive& archive, image_header& header);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive& archive, image_header& header);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive, image_header& header);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive, image_header& header);

bool load_image(const std::filesystem::path& archive_path, image_header& header, std::vector<u8>& pixels)
{
    if (archive_path.extension() == archive_extension<archive_type::binary>()) {
        std::ifstream _fstream(archive_path, std::ios::binary);
        if (_fstream.fail()) {
            return false;
        }
        cereal::BinaryInputArchive _archive(_fstream);
        _archive(header);
        _archive(pixels);
        if (_fstream.fail()) {
            return false;
        }
        return true;

    } else if (archive_path.extension() == archive_extension<archive_type::json>()) {
        std::ifstream _fstream(archive_path, std::ios::binary);
        if (_fstream.fail()) {
            return false;
        }
        cereal::JSONInputArchive _archive(_fstream);
        _archive(header);
        _archive(pixels);
        if (_fstream.fail()) {
            return false;
        }
        return true;

    } else {
        return false;
    }
}

bool save_image(const std::filesystem::path& archive_path, const image_header& header, const std::vector<u8>& pixels)
{
    if (archive_path.extension() == archive_extension<archive_type::binary>()) {
        std::ofstream _fstream(archive_path, std::ios::binary);
        if (_fstream.fail()) {
            return false;
        }
        cereal::BinaryOutputArchive _archive(_fstream);
        _archive(header);
        _archive(pixels);
        if (_fstream.fail()) {
            return false;
        }
        return true;

    } else if (archive_path.extension() == archive_extension<archive_type::json>()) {
        std::ofstream _fstream(archive_path, std::ios::binary);
        if (_fstream.fail()) {
            return false;
        }
        cereal::JSONOutputArchive _archive(_fstream);
        _archive(header);
        _archive(pixels);
        if (_fstream.fail()) {
            return false;
        }
        return true;

    } else {
        return false;
    }
}

}