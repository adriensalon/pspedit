#pragma once

#include <editor/toolchain/docker.hpp>

namespace pspedit {

struct build_configuration {
    std::string docker_image;
    std::optional<std::string> platform;
    std::filesystem::path project_directory;
    std::filesystem::path build_directory;
    std::filesystem::path install_directory;
    std::string build_type = "Debug";
    std::optional<std::size_t> jobs_count = std::nullopt;
    bool is_export_compile_commands = true;
};

struct build_artifacts {
    std::filesystem::path eboot_pbp;
    std::filesystem::path compile_commands;
    std::vector<std::filesystem::path> copied_files;
};

struct build_manager {
    build_manager(const build_configuration& configuration);
    build_manager(const build_manager& other) = delete;
    build_manager& operator=(const build_manager& other) = delete;
    build_manager(build_manager&& other) = default;
    build_manager& operator=(build_manager&& other) = default;

    [[nodiscard]] tool_result configure_project(const docker_manager& docker, const tool_progress_callback& callback = nullptr) const;
    [[nodiscard]] tool_result build_project(const docker_manager& docker, const tool_progress_callback& callback = nullptr) const;

private:
    build_configuration _configuration;
};

}