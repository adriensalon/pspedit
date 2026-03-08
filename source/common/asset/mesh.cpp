#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <common/asset/mesh.hpp>
#include <common/core/archive.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, mesh_asset& image)
{
    archive(cereal::make_nvp("version", image.version));
    archive(cereal::make_nvp("vertex_buffer", image.vertex_buffer));
    archive(cereal::make_nvp("index_buffer", image.index_buffer));
    archive(cereal::make_nvp("vertices", image.vertices));
    archive(cereal::make_nvp("indices", image.indices));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, mesh_asset&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, mesh_asset&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, mesh_asset&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, mesh_asset&);

bool load_asset(const std::filesystem::path& archive_path, mesh_asset& asset)
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

bool save_asset(const std::filesystem::path& archive_path, const mesh_asset& asset)
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