#pragma once

#include <editor/toolchain/build.hpp>
#include <editor/toolchain/ppsspp.hpp>

namespace pspedit {

struct toolchain_configuration {
    std::filesystem::path cache_directory;
    std::filesystem::path workspace_directory;
    std::filesystem::path memory_stick_directory;
    std::string pspdev_image = "pspdev/pspdev:latest";
    std::string docker_executable = "docker";
    bool is_allowed_amd64_on_arm64 = true;
};

struct toolchain_manager {
    toolchain_manager(const toolchain_configuration& configuration);
    toolchain_manager(const toolchain_manager& other) = delete;
    toolchain_manager& operator=(const toolchain_manager& other) = delete;
    toolchain_manager(toolchain_manager&& other) = default;
    toolchain_manager& operator=(toolchain_manager&& other) = default;
    
    docker_manager docker;
    build_manager build;
    ppsspp_manager ppsspp;

    [[nodiscard]] std::vector<editor_action> detect_actions(std::vector<tool_diagnostic>& diagnostics, const tool_progress_callback& callback = nullptr) const;

private:
    toolchain_configuration _configuration;
};

}