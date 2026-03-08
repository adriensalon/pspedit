#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/unordered_map.hpp>

#include <common/asset/scene.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, scene_asset& asset)
{
    archive(cereal::make_nvp("version", asset.version));
    archive(cereal::make_nvp("models", asset.models));
    archive(cereal::make_nvp("transforms", asset.transforms));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, scene_asset&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, scene_asset&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, scene_asset&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, scene_asset&);

bool load_asset(const std::filesystem::path& archive_path, scene_asset& asset)
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

bool save_asset(const std::filesystem::path& archive_path, const scene_asset& asset)
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