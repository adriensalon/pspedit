#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <common/core/texture.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, texture_descriptor& descriptor)
{
    archive(cereal::make_nvp("width", descriptor.width));
    archive(cereal::make_nvp("height", descriptor.height));
    archive(cereal::make_nvp("stride", descriptor.stride));
    archive(cereal::make_nvp("swizzle", descriptor.swizzle));
    archive(cereal::make_nvp("format", descriptor.format));
    archive(cereal::make_nvp("filter_min", descriptor.filter_min));
    archive(cereal::make_nvp("filter_mag", descriptor.filter_mag));
    archive(cereal::make_nvp("wrap_u", descriptor.wrap_u));
    archive(cereal::make_nvp("wrap_v", descriptor.wrap_v));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, texture_descriptor&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, texture_descriptor&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, texture_descriptor&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, texture_descriptor&);

}