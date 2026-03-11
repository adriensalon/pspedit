#pragma once

#include <atomic>
#include <optional>
#include <string>
#include <unordered_map>

#include <common/asset/content.hpp>
#include <common/asset/image.hpp>
#include <common/asset/material.hpp>
#include <common/asset/mesh.hpp>
#include <common/asset/model.hpp>
#include <common/asset/package.hpp>
#include <common/asset/scene.hpp>
#include <common/asset/transform.hpp>
#include <runtime/asset/image.hpp>
#include <runtime/asset/material.hpp>
#include <runtime/asset/mesh.hpp>
#include <runtime/asset/model.hpp>
#include <runtime/asset/package.hpp>
#include <runtime/asset/scene.hpp>
#include <runtime/asset/transform.hpp>

namespace pspedit {

enum struct content_status {
    not_found,
    failed,
    unloading,
    loading,
    staged,
    loaded,
};

template <typename Content, typename ContentAsset>
struct asset_content_slot {
    Content content = {};
    std::optional<ContentAsset> staging = std::nullopt;
    std::atomic<content_status> status = content_status::not_found;
};

struct runtime_content {
    content_asset content;
    std::unordered_map<image_id, asset_content_slot<runtime_image, image_asset>> images;
    std::unordered_map<mesh_id, asset_content_slot<runtime_mesh, mesh_asset>> meshes;
    std::unordered_map<material_id, asset_content_slot<runtime_material, material_asset>> materials;
    std::unordered_map<scene_id, std::optional<scene_asset>> scenes;
    std::unordered_map<package_id, std::optional<package_asset>> packages;
    entt::registry components;
    std::unordered_map<model_id, shared_model> models;
    std::unordered_map<transform_id, shared_transform> transforms;
};

}