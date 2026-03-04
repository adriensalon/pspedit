#pragma once

#include <cstdint>
#include <functional>

struct UsbDriver;
struct SceKernelLMOption;

namespace pspedit {

struct kernel_context {
    std::function<int(const char* path, int flags, SceKernelLMOption* options)> load_module = nullptr;
    std::function<int(int module, unsigned int argc, void* argv, int* status, SceKernelLMOption* options)> start_module = nullptr;
    std::function<int(const char* driver_name, int size, void* args)> usb_start = nullptr;
    std::function<int(const char* driver_name, int size, void* args)> usb_stop = nullptr;
    std::function<int(std::uint32_t pid)> usb_activate = nullptr;
    std::function<int(UsbDriver* driver)> usbbd_register = nullptr;
    std::function<int(unsigned int delay)> delay_thread = nullptr;
};

struct kernel_xploiter {
    kernel_xploiter() = default;
	kernel_xploiter(const kernel_xploiter& other) = delete;
	kernel_xploiter& operator=(const kernel_xploiter& other) = delete;
	kernel_xploiter(kernel_xploiter&& other) = default;
	kernel_xploiter& operator=(kernel_xploiter&& other) = default;

    bool run_kernel(const std::function<void(kernel_context&)>& callback);
};
}