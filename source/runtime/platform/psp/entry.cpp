#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkernel.h>

PSP_MODULE_INFO("pspedit_game", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define BUFFER_WIDTH 512
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

// GU command list
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

    sceGuInit();
    sceGuStart(GU_DIRECT, _gu_command_list);
    sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)(BUFFER_WIDTH * SCREEN_HEIGHT * 4), BUFFER_WIDTH);
    sceGuDepthBuffer((void*)(2 * BUFFER_WIDTH * SCREEN_HEIGHT * 4), BUFFER_WIDTH);
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDepthMask(GU_TRUE);
    sceGuDisable(GU_DEPTH_TEST);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    int running = 1;
    while (running) {
        sceGuStart(GU_DIRECT, _gu_command_list);
        sceGuClearColor(0xFF0000FF);
        sceGuClear(GU_COLOR_BUFFER_BIT);
        

        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }

    sceGuTerm();
    sceKernelExitGame();
    return 0;
}