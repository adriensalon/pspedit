#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <common/core/buffer.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, vertex_buffer_descriptor& descriptor)
{
	archive(cereal::make_nvp("vertex", descriptor.vertex));
	archive(cereal::make_nvp("usage", descriptor.usage));
	archive(cereal::make_nvp("count", descriptor.count));
}

template <typename Archive>
void serialize(Archive& archive, index_buffer_descriptor& descriptor)
{
	archive(cereal::make_nvp("usage", descriptor.usage));
	archive(cereal::make_nvp("format", descriptor.format));
	archive(cereal::make_nvp("count", descriptor.count));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, vertex_buffer_descriptor&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, vertex_buffer_descriptor&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, vertex_buffer_descriptor&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, vertex_buffer_descriptor&);

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, index_buffer_descriptor&);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, index_buffer_descriptor&);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, index_buffer_descriptor&);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, index_buffer_descriptor&);

}