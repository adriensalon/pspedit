#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>

#include <common/core/vertex.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, vertex_attribute& attribute)
{
	archive(cereal::make_nvp("semantic", attribute.semantic));
	archive(cereal::make_nvp("storage", attribute.storage));
	archive(cereal::make_nvp("components", attribute.components));
	archive(cereal::make_nvp("offset", attribute.offset));
}

template <typename Archive>
void serialize(Archive& archive, vertex_descriptor& descriptor)
{
	archive(cereal::make_nvp("stride", descriptor.stride));
	archive(cereal::make_nvp("morph_targets", descriptor.morph_targets));
	archive(cereal::make_nvp("skin_weights", descriptor.skin_weights));
	archive(cereal::make_nvp("attributes_count", descriptor.attributes_count));
	archive(cereal::make_nvp("attributes", descriptor.attributes));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, vertex_descriptor&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, vertex_descriptor&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, vertex_descriptor&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, vertex_descriptor&);

}