#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/optional.hpp>

#include <common/asset/material.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, material_asset& asset)
{
    archive(cereal::make_nvp("version", asset.version));
    archive(cereal::make_nvp("pipeline", asset.pipeline));
    archive(cereal::make_nvp("function", asset.function));
    archive(cereal::make_nvp("image", asset.image));
    archive(cereal::make_nvp("is_texture_enabled", asset.is_texture_enabled));
    archive(cereal::make_nvp("is_lighting_enabled", asset.is_lighting_enabled));
    archive(cereal::make_nvp("use_vertex_color", asset.use_vertex_color));
    archive(cereal::make_nvp("ambient_color", asset.ambient_color));
    archive(cereal::make_nvp("diffuse_color", asset.diffuse_color));
    archive(cereal::make_nvp("specular_color", asset.specular_color));
    archive(cereal::make_nvp("emissive_color", asset.emissive_color));
    archive(cereal::make_nvp("shininess", asset.shininess));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive& archive, material_asset& asset);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive& archive, material_asset& asset);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive, material_asset& asset);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive, material_asset& asset);

bool load_asset(const std::filesystem::path& archive_path, material_asset& asset)
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

bool save_asset(const std::filesystem::path& archive_path, const material_asset& asset)
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