#pragma once

#include <filesystem>

#include <editor/core/action.hpp>
#include <editor/core/tool.hpp>

namespace pspedit {

struct ppsspp_configuration {
    std::filesystem::path install_directory;
    std::filesystem::path memory_stick_directory;
};

struct ppsspp_info {
    bool is_executable_found = false;
    std::filesystem::path executable;
};

struct ppsspp_manager {
    ppsspp_manager(const ppsspp_configuration& configuration);
    ppsspp_manager(const ppsspp_manager& other) = delete;
    ppsspp_manager& operator=(const ppsspp_manager& other) = delete;
    ppsspp_manager(ppsspp_manager&& other) = default;
    ppsspp_manager& operator=(ppsspp_manager&& other) = default;

    [[nodiscard]] ppsspp_info probe_info() const;
    [[nodiscard]] tool_result launch_game(const std::filesystem::path& game_directory, const std::vector<std::string>& args = {}) const;

private:
    ppsspp_configuration _configuration;
};

}