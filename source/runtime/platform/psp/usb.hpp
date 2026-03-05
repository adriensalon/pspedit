#pragma once

#include <platform/psp/kernel.hpp>

namespace pspedit {

bool kernel_start_usb(kernel_context& context);
bool kernel_stop_usb(kernel_context& context);

}