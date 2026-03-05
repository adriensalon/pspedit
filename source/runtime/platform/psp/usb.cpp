
#include <pspdebug.h>
#include <pspmodulemgr.h>
#include <pspusb.h>
#include <pspusbbus.h>
#include <pspusbstor.h>

#include <platform/psp/usb.hpp>

namespace pspedit {
namespace {

    void _load_and_start_module(pspedit::kernel_context& context, const char* path)
    {
        const SceUID _module_id = context.load_module(path, 0, NULL);
        if (_module_id < 0) {
            pspDebugScreenPrintf("Failed to load %s module\n", path);
            return;
        }

        int _status;
        if (context.start_module(_module_id, 0, NULL, &_status, NULL) < 0) {
            pspDebugScreenPrintf("Failed to start %s module\n", path);
        }

        pspDebugScreenPrintf("Loaded and started module %s\n", path);
    }

}

bool kernel_start_usb(kernel_context& context)
{
    _load_and_start_module(context, "flash0:/kd/semawm.prx");
    _load_and_start_module(context, "flash0:/kd/usbstor.prx");
    _load_and_start_module(context, "flash0:/kd/usbstormgr.prx");
    _load_and_start_module(context, "flash0:/kd/usbstorms.prx");
    int _usb_result;

    _usb_result = context.usb_start(PSP_USBBUS_DRIVERNAME, 0, 0);
    pspDebugScreenPrintf("Tried USB bus driver registration with result 0x%08X\n", _usb_result);
    
	_usb_result = context.usb_start(PSP_USBSTOR_DRIVERNAME, 0, 0);
	pspDebugScreenPrintf("Tried USB mass storage registration with result 0x%08X\n", _usb_result);

    // _usb_result = context.usb_activate(0x1C00);
	// pspDebugScreenPrintf("Tried USB activation with result 0x%08X\n", _usb_result);

    return (_usb_result >= 0);
}

// bool kernel_stop_usb(kernel_context& context)
// {
//     _load_and_start_module(context, "flash0:/kd/semawm.prx");
//     _load_and_start_module(context, "flash0:/kd/usbstor.prx");
//     _load_and_start_module(context, "flash0:/kd/usbstormgr.prx");
//     _load_and_start_module(context, "flash0:/kd/usbstorms.prx");

//     // TODO
//     return false;
// }

}