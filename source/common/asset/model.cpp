#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <common/asset/model.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, model_asset& asset)
{
    archive(cereal::make_nvp("version", asset.version));
    archive(cereal::make_nvp("material", asset.material));
    archive(cereal::make_nvp("mesh", asset.mesh));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive& archive, model_asset& asset);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive& archive, model_asset& asset);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive, model_asset& asset);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive, model_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, model_asset& asset)
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

bool save_asset(const std::filesystem::path& archive_path, const model_asset& asset)
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