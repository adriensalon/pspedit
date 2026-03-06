
#include <pspdisplay.h>
#include <pspgu.h>

#include <runtime/core/buffer.hpp>
#include <runtime/core/framebuffer.hpp>
#include <runtime/core/graphics.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] int _to_gu_psm(const pixel_format format)
    {
        switch (format) {
        case pixel_format::rgb565:
            return GU_PSM_5650;
        case pixel_format::rgba4444:
            return GU_PSM_4444;
        case pixel_format::rgba5551:
            return GU_PSM_5551;
        case pixel_format::rgba8888:
            return GU_PSM_8888;
        case pixel_format::r8:
            return GU_PSM_T8;
        case pixel_format::r16:
            return GU_PSM_T16;
        case pixel_format::r32:
            return GU_PSM_T32;
        default:
            return GU_PSM_8888;
        }
    }

    static u32 _to_gu_vertex_format(const vertex_descriptor& descriptor)
    {
        u32 _format = 0;
        for (u8 _attribute_index = 0; _attribute_index < descriptor.attrib_count; ++_attribute_index) {
            const vertex_attribute& _attribute = descriptor.attributes[_attribute_index];

            switch (_attribute.semantic) {
            case vertex_semantic::position:
                if (_attribute.storage == vertex_storage::f32) {
                    _format |= GU_VERTEX_32BITF;
                }
                break;

            case vertex_semantic::color:
                if (_attribute.storage == vertex_storage::normalized_u8 && _attribute.components == 4) {
                    _format |= GU_COLOR_8888;
                }
                break;

            default:
                break;
            }
        }

        _format |= GU_TRANSFORM_3D;
        return _format;
    }

}

graphics_context::graphics_context(framebuffer& target, void* command_list, const std::size_t command_list_bytes)
    : _command_list(command_list)
    , _command_list_bytes(command_list_bytes)
{
    sceGuInit();
    begin_frame(target);
    end_frame(target);
    sceGuDisplay(GU_TRUE);
}

graphics_context::~graphics_context()
{
    sceGuTerm();
}

void graphics_context::begin_frame(const framebuffer& target)
{
    const framebuffer_descriptor& _descriptor = target.descriptor();

    sceGuStart(GU_DIRECT, _command_list);
    sceGuDrawBuffer(static_cast<int>(_to_gu_psm(_descriptor.format)), reinterpret_cast<void*>(static_cast<std::uintptr_t>(target.draw_buffer())), static_cast<int>(_descriptor.buffer_width));
    sceGuDispBuffer(static_cast<int>(_descriptor.width), static_cast<int>(_descriptor.height), reinterpret_cast<void*>(static_cast<std::uintptr_t>(target.display_buffer())), static_cast<int>(_descriptor.buffer_width));

    if (_descriptor.has_depth) {
        sceGuDepthBuffer(reinterpret_cast<void*>(static_cast<std::uintptr_t>(target.depth_buffer())), static_cast<int>(_descriptor.buffer_width));
    }

    sceGuOffset(static_cast<int>(_descriptor.offset_x), static_cast<int>(_descriptor.offset_y));
    sceGuViewport(2048, 2048, static_cast<int>(_descriptor.width), static_cast<int>(_descriptor.height));
    sceGuScissor(0, 0, static_cast<int>(_descriptor.width), static_cast<int>(_descriptor.height));
    sceGuEnable(GU_SCISSOR_TEST);
}

void graphics_context::end_frame(framebuffer& target)
{
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

void graphics_context::clear(const u32 clear_flags, const u32 color, const f32 depth, const u32 stencil)
{
    sceGuClearColor(color);
    // sceGuClearDepth(static_cast<int>(depth * 0xFFFF)); // GU depth is typically 16-bit
    // sceGuClearStencil(stencil);
    sceGuClear(clear_flags);
}

void graphics_context::draw(const vertex_buffer& buffer)
{
    const vertex_descriptor& _vertex_descriptor = buffer.descriptor().vertex;
    const u32 _buffer_count = buffer.descriptor().count;
    const u32 _format = _to_gu_vertex_format(_vertex_descriptor);

    sceGuDisable(GU_TEXTURE_2D);
    sceGuShadeModel(GU_SMOOTH);
    sceGuDrawArray(GU_TRIANGLES, _format, _buffer_count, 0, buffer.data());
}

}