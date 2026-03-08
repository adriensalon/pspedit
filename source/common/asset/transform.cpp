#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/vector.hpp>

#include <common/asset/transform.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, transform_asset& asset)
{
    archive(cereal::make_nvp("version", asset.version));
    archive(cereal::make_nvp("trs", asset.trs));
    archive(cereal::make_nvp("parent", asset.parent));
    archive(cereal::make_nvp("children", asset.children));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive& archive, transform_asset& asset);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive& archive, transform_asset& asset);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive, transform_asset& asset);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive, transform_asset& asset);

bool load_transform(const std::filesystem::path& archive_path, transform_asset& asset)
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

bool save_transform(const std::filesystem::path& archive_path, const transform_asset& asset)
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