#pragma once

#include <filesystem>

#include <editor/core/tool.hpp>

namespace pspedit {

struct docker_info {
    bool is_executable_found = false;
    bool is_daemon_reachable = false;
    std::string version;
    std::string server_version;
};

struct docker_image_info {
    std::string name;
    std::string id;
    std::string created;
    std::string size;
    bool is_present = false;
};

struct docker_command {
    std::string image;
    std::filesystem::path workdir_host;
    std::string workdir_container = "/work";
    std::vector<std::pair<std::filesystem::path, std::string>> mounts;
    std::vector<std::pair<std::string, std::string>> env;
    std::vector<std::string> command;
    bool is_remove_after = true;
    std::optional<std::string> platform;
};

struct docker_manager {
    docker_manager(const std::string& executable);
    docker_manager(const docker_manager& other) = delete;
    docker_manager& operator=(const docker_manager& other) = delete;
    docker_manager(docker_manager&& other) = default;
    docker_manager& operator=(docker_manager&& other) = default;

    [[nodiscard]] docker_info probe_info() const;
    [[nodiscard]] docker_image_info inspect_image(const std::string& image) const;
    [[nodiscard]] tool_result pull_image(const std::string& image, const tool_progress_callback& callback = nullptr) const;
    [[nodiscard]] tool_result run_command(const docker_command& command, const tool_progress_callback& callback = nullptr) const;

private:
    std::string _executable;
};

}