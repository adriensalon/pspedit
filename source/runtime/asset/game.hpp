#pragma once

#include <common/asset/game.hpp>
#include <runtime/core/image.hpp>
#include <runtime/core/mesh.hpp>
#include <runtime/core/material.hpp>

namespace pspedit {

struct vram_allocator;

struct game {
	game(vram_allocator& allocator, const game_asset& asset);
    game(const game& other) = delete;
    game& operator=(const game& other) = delete;
    game(game&& other) noexcept = default;
    game& operator=(game&& other) noexcept = delete;
	~game();

	void load_package_async();


private:
	std::unordered_map<image_id, image> _loaded_images = {};
	std::unordered_map<mesh_id, mesh> _loaded_meshes = {};
	std::unordered_map<material_id, material> _loaded_materials = {};
	std::unordered_map<scene_id, scene> _loaded_scenes = {};
	game_asset _asset;
};

}