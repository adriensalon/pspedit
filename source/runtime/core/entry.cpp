#include <pspdebug.h>
#include <pspkernel.h>
#include <pspgu.h>

#include <runtime/core/framebuffer.hpp>
#include <runtime/core/swapchain.hpp>
#include <runtime/core/usb.hpp>
#include <runtime/core/vram.hpp>

PSP_MODULE_INFO("pspedit_game", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

static unsigned int __attribute__((aligned(16))) _gu_command_list[262144];

int _exit_callback(int arg1, int arg2, void* common)
{
    sceKernelExitGame();
    return 0;
}

int _callback_thread(SceSize args, void* argp)
{
    const int _cbid = sceKernelCreateCallback("exit_callback", _exit_callback, nullptr);
    sceKernelRegisterExitCallback(_cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int main()
{
    pspDebugScreenInit();
    const int _thread_id = sceKernelCreateThread("update_thread", _callback_thread, 0x11, 0xFA0, 0, 0);
    if (_thread_id >= 0) {
        sceKernelStartThread(_thread_id, 0, nullptr);
    }

    pspedit::vram_allocator _vram;
    pspDebugScreenPrintf("Created VRAM allocator\n");
    pspedit::framebuffer _default_framebuffer = pspedit::framebuffer::default_framebuffer(_vram);
    pspDebugScreenPrintf("Created default framebuffer\n");
    pspedit::swapchain _context(_default_framebuffer, _gu_command_list, sizeof(_gu_command_list));
    pspDebugScreenPrintf("Created graphics context\n");
	
    int running = 1;
    while (running) {

        _context.begin_frame(_default_framebuffer);
        _context.clear(GU_COLOR_BUFFER_BIT, 0xFF0000FF, 1.0f);
        _context.end_frame(_default_framebuffer);
    }

    sceKernelExitGame();
    return 0;
}