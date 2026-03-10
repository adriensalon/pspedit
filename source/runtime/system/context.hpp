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

struct vram_allocator;

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
    std::atomic<content_status> status = content_status::not_found;
    std::optional<Content> content = std::nullopt;
    std::optional<ContentAsset> staging = std::nullopt;
};

template <typename Id>
struct gamesave_request_delta {
    std::vector<Id> created_ids = {};
    std::vector<Id> modified_ids = {};
    std::vector<Id> destroyed_ids = {};
};

struct gamesave_request {
    gamesave_request_delta<image_id> images = {};
    gamesave_request_delta<mesh_id> meshes = {};
    gamesave_request_delta<transform_id> transforms = {};
};

struct context {
    context(vram_allocator& allocator, const content_asset& content);

    [[nodiscard]] image* resolve_image(const image_id id);
    [[nodiscard]] material* resolve_material(const material_id id);
    [[nodiscard]] mesh* resolve_mesh(const mesh_id id);
    [[nodiscard]] model* resolve_model(const model_id id);
    [[nodiscard]] transform* resolve_transform(const transform_id id);

    void load_package_(const package_id id);
    void load_scene_async(const scene_id id);
    void unload_package(const package_id id);
    void unload_scene(const scene_id id);

    [[nodiscard]] content_status package_status(const package_id id) const;
    [[nodiscard]] content_status scene_status(const scene_id id) const;

    void load_gamesave_async(const std::string& gamesave_name);
    void save_gamesave_async(const std::string& gamesave_name, const gamesave_request& request);

private:
    content_asset _content;
    std::unordered_map<image_id, asset_content_slot<image, image_asset>> _loaded_images;
    std::unordered_map<mesh_id, asset_content_slot<mesh, mesh_asset>> _loaded_meshes;
    std::unordered_map<material_id, asset_content_slot<material, material_asset>> _loaded_materials;
    std::unordered_map<scene_id, asset_content_slot<scene, scene_asset>> _loaded_scenes;
    std::unordered_map<package_id, asset_content_slot<package, package_asset>> _loaded_packages;
	entt::registry _loaded_components;
    std::unordered_map<model_id, model*> _loaded_models;
    std::unordered_map<transform_id, transform*> _loaded_transforms;
};

}