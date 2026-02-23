#pragma once

#include <filesystem>

[[nodiscard]] bool is_docker_found();
[[nodiscard]] bool is_docker_running();
[[nodiscard]] bool is_docker_image_installed(const std::string& image_name = "pspdev/pspdev:latest");
[[nodiscard]] std::string docker_version();
[[nodiscard]] std::string docker_engine_version();
void pull_docker_image(const std::string& image_name = "pspdev/pspdev:latest");
// run commands
