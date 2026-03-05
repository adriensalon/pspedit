
#include <pspdisplay.h>
#include <pspgu.h>

#include <platform/psp/graphics.hpp>

namespace pspedit {
namespace {

}

graphics_context::graphics_context(void* command_list, const std::size_t command_list_bytes)
    : _command_list(command_list)
    , _command_list_bytes(command_list_bytes)
{
    sceGuInit();
}

graphics_context::~graphics_context()
{
    sceGuTerm();
}

void graphics_context::begin_frame(const render_target& target)
{
    sceGuStart(GU_DIRECT, _command_list);
    bind_render_target(target);
    setup_viewport(target);
}

void graphics_context::end_frame()
{
    finish_and_sync();
    swap_buffers();
}

void graphics_context::clear(const u32 clear_flags, const u32 color, const f32 depth, const u32 stencil)
{
    sceGuClearColor(color);
    sceGuClearDepth(static_cast<int>(depth * 0xFFFF)); // GU depth is typically 16-bit
    sceGuClearStencil(stencil);
    sceGuClear(clear_flags);
}

void bind_pipeline(const pipeline_state& pipeline)
{

}

void graphics_context::bind_render_target(const render_target& target)
{
    sceGuDrawBuffer(static_cast<int>(target.format), target.draw_buffer, static_cast<int>(target.buffer_width));
    sceGuDispBuffer(static_cast<int>(target.width), static_cast<int>(target.height), target.display_buffer, static_cast<int>(target.buffer_width));
    if (target.depth_buffer) {
        sceGuDepthBuffer(target.depth_buffer, static_cast<int>(target.buffer_width));
    }
}

void graphics_context::setup_viewport(const render_target& target)
{
    sceGuOffset(static_cast<int>(target.offset_x), static_cast<int>(target.offset_y));
    sceGuViewport(2048, 2048, static_cast<int>(target.width), static_cast<int>(target.height));
    sceGuScissor(0, 0, static_cast<int>(target.width), static_cast<int>(target.height));
    sceGuEnable(GU_SCISSOR_TEST);
}

void graphics_context::finish_and_sync()
{
    sceGuFinish();
    sceGuSync(0, 0);
}

void graphics_context::swap_buffers()
{
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

}