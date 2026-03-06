#pragma once

namespace pspedit {

struct kernel_context;

bool kernel_start_usb(kernel_context& context);
bool kernel_stop_usb(kernel_context& context);

}