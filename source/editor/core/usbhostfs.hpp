#pragma once

#include <chrono>
#include <filesystem>

struct usbhostfs_options {
    std::size_t drive_index = 0;
    std::filesystem::path drive_host_path = "";
    std::uint16_t base_port = 10000;
    bool is_global_bound = false;
    bool is_nocase_enabled = false;
    std::chrono::milliseconds usb_timeout  { 0 };
    bool is_verbose_enabled = false;
};

[[nodiscard]] bool usbhostfs_start(const usbhostfs_options& options = {});
