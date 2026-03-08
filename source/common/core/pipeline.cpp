#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <common/core/pipeline.hpp>

namespace pspedit {

template <typename Archive>
void serialize(Archive& archive, pipeline_descriptor& descriptor)
{
	archive(cereal::make_nvp("cull", descriptor.cull));
	archive(cereal::make_nvp("is_depth_test_enabled", descriptor.is_depth_test_enabled));
	archive(cereal::make_nvp("is_depth_write_enabled", descriptor.is_depth_write_enabled));
	archive(cereal::make_nvp("depth_operation", descriptor.depth_operation));
	archive(cereal::make_nvp("is_blend_enabled", descriptor.is_blend_enabled));
	archive(cereal::make_nvp("blend", descriptor.blend));
	archive(cereal::make_nvp("blend_source", descriptor.blend_source));
	archive(cereal::make_nvp("blend_destination", descriptor.blend_destination));
	archive(cereal::make_nvp("is_dither_enabled", descriptor.is_dither_enabled));
	archive(cereal::make_nvp("is_alphatest_enabled", descriptor.is_alphatest_enabled));
	archive(cereal::make_nvp("alphatest_reference", descriptor.alphatest_reference));
	archive(cereal::make_nvp("alphatest_operation", descriptor.alphatest_operation));
	archive(cereal::make_nvp("is_smooth_shading_enabled", descriptor.is_smooth_shading_enabled));
}

template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive& archive, pipeline_descriptor& descriptor);
template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive& archive, pipeline_descriptor& descriptor);
template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive, pipeline_descriptor& descriptor);
template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive, pipeline_descriptor& descriptor);

}