
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

void graphics_context::clear(const u32 clearFlags, const u32 color, const f32 depth, const u32 stencil)
{
    sceGuClearColor(color);
    sceGuClearDepth(static_cast<int>(depth * 0xFFFF)); // GU depth is typically 16-bit
    sceGuClearStencil(stencil);
    sceGuClear(clearFlags);
}

void graphics_context::setup_viewport(const render_target& target)
{
    sceGuOffset(target.offsetX, target.offsetY);
    sceGuViewport(2048, 2048, target.width, target.height);
    sceGuScissor(0, 0, target.width, target.height);
    sceGuEnable(GU_SCISSOR_TEST);
}

void graphics_context::bind_render_target(const render_target& target)
{
    sceGuDrawBuffer(target.pixelFormat, target.draw_buffer, target.bufferWidth);
    sceGuDispBuffer(target.width, target.height, target.display_buffer, target.bufferWidth);
    if (target.depth_buffer) {
        sceGuDepthBuffer(target.depth_buffer, target.bufferWidth);
    }
}

}