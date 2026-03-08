#pragma once

#include <common/asset/game.hpp>

namespace pspedit {

struct vram_allocator;

struct game {
	game(vram_allocator& allocator, const game_asset& asset);
    game(const game& other) = delete;
    game& operator=(const game& other) = delete;
    game(game&& other) noexcept = default;
    game& operator=(game&& other) noexcept = delete;
	~game();

private:
	game_asset _asset;
};

}