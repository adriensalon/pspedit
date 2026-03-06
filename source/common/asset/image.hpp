#pragma once

#include <common/core/texture.hpp>

namespace pspedit {

	struct image_asset_header {
		// meta
		texture_descriptor texture;
	};

	struct image_asset {
		image_asset_header header;
		void* pixels;
	};

}