#pragma once

#include <runtime/asset/content.hpp>

namespace pspedit {
	

// template <typename Id>
// struct gamesave_request_delta {
//     std::vector<Id> created_ids = {};
//     std::vector<Id> modified_ids = {};
//     std::vector<Id> destroyed_ids = {};
// };

// struct gamesave_request {
//     gamesave_request_delta<image_id> images = {};
//     gamesave_request_delta<mesh_id> meshes = {};
//     gamesave_request_delta<transform_id> transforms = {};
// };

struct context {
    context(vram_allocator& allocator, const content_asset& content);

    // [[nodiscard]] image* resolve_image(const image_id id);
    // [[nodiscard]] material* resolve_material(const material_id id);
    // [[nodiscard]] mesh* resolve_mesh(const mesh_id id);
    // [[nodiscard]] model* resolve_model(const model_id id);
    // [[nodiscard]] transform* resolve_transform(const transform_id id);

    // void load_package_(const package_id id);
    // void load_scene_async(const scene_id id);
    // void unload_package(const package_id id);
    // void unload_scene(const scene_id id);

    // [[nodiscard]] content_status package_status(const package_id id) const;
    // [[nodiscard]] content_status scene_status(const scene_id id) const;

    // void load_gamesave_async(const std::string& gamesave_name);
    // void save_gamesave_async(const std::string& gamesave_name, const gamesave_request& request);

	runtime_content content;
};

}