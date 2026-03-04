#include <string>

extern "C" {
#include <libpspexploit.h>
}

#include <pspmodulemgr.h>
#include <pspusb.h>
#include <pspusbbus.h>
#include <pspusbstor.h>

#include <platform/psp/kernel.hpp>

#define NID_sceKernelLoadModule 0xFFB9B760
#define NID_sceKernelStartModule 0xE6BF3960
#define NID_sceUsbStart 0xAE5DE6AF
#define NID_sceUsbStop 0xC2464FA0
#define NID_sceUsbActivate 0x586DB82C
#define NID_sceUsbbdRegister 0xB1644BE7

namespace pspedit {
namespace {

    static std::function<void(kernel_context&)> _userland_callback = nullptr;

    template <typename Return, typename... Args>
    bool _ensure_function_found(std::function<Return(Args...)>& function_ref, const std::string& module_name, const std::string& library_name, const std::size_t nid)
    {
        using FunctionPtr = Return (*)(Args...);

        u32 _found_address = pspXploitFindFunction(module_name.c_str(), library_name.c_str(), nid);
        if (!_found_address) {
            pspDebugScreenPrintf("Failed to load function from %s at 0x%08X\n", library_name.c_str(), _found_address);
            return false;
        }

        function_ref = reinterpret_cast<FunctionPtr>(static_cast<std::uintptr_t>(_found_address));
        pspDebugScreenPrintf("Found kernel function from %s at 0x%08X\n", library_name.c_str(), _found_address);
        return true;
    }

    void _kernel_callback()
    {
        const int _memory_k1 = pspSdkSetK1(0);
        const int _memory_level = pspXploitSetUserLevel(8);

        KernelFunctions _kernel_functions;
        pspXploitScanKernelFunctions(&_kernel_functions);

        kernel_context _kernel_context;
        _ensure_function_found(_kernel_context.load_module, "sceModuleManager", "ModuleMgrForKernel", NID_sceKernelLoadModule);
        _ensure_function_found(_kernel_context.start_module, "sceModuleManager", "ModuleMgrForKernel", NID_sceKernelStartModule);
        _ensure_function_found(_kernel_context.usb_start, "sceUSB_Driver", "sceUsb", NID_sceUsbStart);
        _ensure_function_found(_kernel_context.usb_stop, "sceUSB_Driver", "sceUsb", NID_sceUsbStop);
        _ensure_function_found(_kernel_context.usb_activate, "sceUSB_Driver", "sceUsb", NID_sceUsbActivate);
        _ensure_function_found(_kernel_context.usbbd_register, "sceUSB_Driver", "sceUsbBus_driver", NID_sceUsbbdRegister);
        _kernel_context.delay_thread = _kernel_functions.KernelDelayThread;

        if (_userland_callback) {
            _userland_callback(_kernel_context);
        }

        pspXploitRepairKernel();
        pspXploitSetUserLevel(_memory_level);
        pspSdkSetK1(_memory_k1);
    }

}

bool kernel_xploiter::run_kernel(const std::function<void(kernel_context&)>& callback)
{
    static bool _is_xploiter_setup = false;
    if (!_is_xploiter_setup) {
        if (pspXploitInitKernelExploit() < 0) {
            return false;
        }
        _is_xploiter_setup = true;
    }

    if (pspXploitDoKernelExploit() < 0) {
        return false;
    }

    _userland_callback = callback;
    pspXploitExecuteKernel(reinterpret_cast<void*>(&_kernel_callback));
    return true;
}

}