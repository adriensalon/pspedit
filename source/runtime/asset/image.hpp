#pragma once

#include <common/asset/image.hpp>

namespace pspedit {

using shared_image_reference = u32;

struct shared_image {


private:
	image_asset_header* _header;
	void* _pixels;
};

}