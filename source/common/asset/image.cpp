#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <common/asset/image.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, image_asset& image)
{
    archive(cereal::make_nvp("version", image.version));
    archive(cereal::make_nvp("texture", image.texture));
    archive(cereal::make_nvp("pixels", image.pixels));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, image_asset&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, image_asset&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, image_asset&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, image_asset&);

bool load_asset(const std::filesystem::path& archive_path, image_asset& asset)
{
    if (archive_path.extension() == archive_extension<archive_type::binary>()) {
        std::ifstream _fstream(archive_path, std::ios::binary);
        if (_fstream.fail()) {
            return false;
        }
        cereal::BinaryInputArchive _archive(_fstream);
        _archive(asset);
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
        _archive(asset);
        if (_fstream.fail()) {
            return false;
        }
        return true;

    } else {
        return false;
    }
}

bool save_asset(const std::filesystem::path& archive_path, const image_asset& asset)
{
    if (archive_path.extension() == archive_extension<archive_type::binary>()) {
        std::ofstream _fstream(archive_path, std::ios::binary);
        if (_fstream.fail()) {
            return false;
        }
        cereal::BinaryOutputArchive _archive(_fstream);
        _archive(asset);
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
        _archive(asset);
        if (_fstream.fail()) {
            return false;
        }
        return true;

    } else {
        return false;
    }
}

}