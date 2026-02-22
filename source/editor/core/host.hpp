#pragma once

#include <string>

namespace pspedit {

enum struct host_os {
    windows,
    macos,
    linux
};

enum struct host_arch {
    x64,
    arm64
};

struct host_info {
    host_os os;
    host_arch arch;
    std::string os_version;
};

}