

#include <pspge.h>
#include <pspgu.h>
#include <pspkernel.h>

#include <pspedit/core/texture.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] static int _to_gu(const texture_format format)
    {
        switch (f) {
        case texture_format::rgba5650:
            return GU_PSM_5650;
        case texture_format::rgba5551:
            return GU_PSM_5551;
        case texture_format::rgba4444:
            return GU_PSM_4444;
        }
        return GU_PSM_5650;
    }
}

void texture_handle::transfer(const texture_storage storage)
{
    if (storage != _descriptor.storage) {
        if (storage == texture_storage::ram) {

        } else if (storage == texture_storage::vram) {
        }
    }
}

void texture_handle::bind();

}
