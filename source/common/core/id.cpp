#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <common/core/id.hpp>

#define _CEREAL_ID_IMPLEMENTATION(Type)                                                        \
    template <typename Archive>                                                                \
    void serialize(Archive& archive, Type& id)                                                 \
    {                                                                                          \
        archive(cereal::make_nvp("value", id.value));                                          \
    }                                                                                          \
    template void serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, Type&);   \
    template void serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, Type&); \
    template void serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, Type&);       \
    template void serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, Type&);

namespace pspedit {

_CEREAL_ID_IMPLEMENTATION(image_id)
_CEREAL_ID_IMPLEMENTATION(mesh_id)
_CEREAL_ID_IMPLEMENTATION(material_id)
_CEREAL_ID_IMPLEMENTATION(package_id)
_CEREAL_ID_IMPLEMENTATION(model_id)
_CEREAL_ID_IMPLEMENTATION(transform_id)
_CEREAL_ID_IMPLEMENTATION(scene_id)

}