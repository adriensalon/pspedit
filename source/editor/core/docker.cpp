#include <optional>
#include <sstream>

#include <editor/core/docker.hpp>
#include <editor/core/log.hpp>
#include <editor/core/tool.hpp>

namespace {

static constexpr const char* _pspdev_docker_image = "pspdev/pspdev:latest";
static bool _is_docker_installed = false;

[[nodiscard]] static std::string _normalize_newline(const std::string& unformatted)
{
    std::string _formatted = trim(unformatted);
    const std::size_t _position = _formatted.find('\n');
    if (_position != std::string::npos) {
        _formatted.resize(_position);
    }
    return trim(_formatted);
}

[[nodiscard]] static bool _ensure_docker_installed()
{
    if (!_is_docker_installed) {
        const tool_result _version_result = run_capture_combined("docker", { "--version" });
        if (_version_result.exit_code != 0) {
            log_error("Build", "Docker not found please install from https://www.docker.com");
            return false;
        } else {
            const std::string _docker_version = _normalize_newline(_version_result.stdout_text);
            log_message("Build", "Found " + _docker_version);
        }
        _is_docker_installed = true;
    }
    return true;
}

[[nodiscard]] static bool _ensure_docker_reachable()
{
    const tool_result _reachable_result = run_capture_combined("docker", { "info" });
    if (_reachable_result.exit_code != 0) {
        log_error("Build", "Docker not running please start the engine");
        return false;
    } else {
        const tool_result _docker_engine_version_result = run_capture_combined("docker", { "version", "--format", "{{.Server.Version}}" });
        const std::string _docker_engine_version = _normalize_newline(_docker_engine_version_result.stdout_text);
        log_message("Build", "Found Docker engine running with version " + _docker_engine_version);
    }
    return true;
}

[[nodiscard]] static bool _ensure_docker_image_pulled()
{
    tool_result _inspect_result = run_capture_combined("docker", { "image", "inspect", _pspdev_docker_image, "--format", "{{.Id}}|{{.Created}}|{{.Size}}" });
    if (_inspect_result.exit_code != 0) {
        const tool_result _pull_result = run_capture_combined("docker", { "pull", _pspdev_docker_image });
        if (_pull_result.exit_code != 0) {
            log_error("Build", "Failed to fetch " + std::string(_pspdev_docker_image) + " image");
            return false;
        }
        log_message("Build", "Fetched " + std::string(_pspdev_docker_image) + " docker image");
        _inspect_result = run_capture_combined("docker", { "image", "inspect", _pspdev_docker_image, "--format", "{{.Id}}|{{.Created}}|{{.Size}}" });
    }
    const std::string _inspect_line = _normalize_newline(_inspect_result.stdout_text);
    std::istringstream _inspect_line_stream(_inspect_line);
    std::string _image_id, _image_created, _image_size;
    std::getline(_inspect_line_stream, _image_id, '|');
    std::getline(_inspect_line_stream, _image_created, '|');
    std::getline(_inspect_line_stream, _image_size, '|');
    const std::string _formatted_size = std::to_string(std::stoull(trim(_image_size)) / static_cast<std::size_t>(1e6)) + "Mb";
    log_message("Build", "Found Docker image from " + std::string(_pspdev_docker_image) + " (" + _formatted_size + ")");
    return true;
}

}

bool docker_build_project(const std::filesystem::path& source_directory, const std::filesystem::path& build_directory, const std::optional<std::filesystem::path>& custom_pspedit_dir)
{
    if (!_ensure_docker_installed()) {
        return false;
    }
    if (!_ensure_docker_reachable()) {
        return false;
    }
    if (!_ensure_docker_image_pulled()) {
        return false;
    }

    // Ensure absolute paths for docker bind mounts
	const std::filesystem::path src_abs = custom_pspedit_dir ? custom_pspedit_dir.value() : std::filesystem::absolute(source_directory);
    const auto bld_abs = std::filesystem::absolute(build_directory);

    // Make sure build dir exists on host
    std::error_code ec;
    std::filesystem::create_directories(bld_abs, ec);

    // Container mount points
    // const std::string src_mnt = "/source";
    // const std::string bld_mnt = "/build";

    std::string script;
    if (custom_pspedit_dir) {

        script = "set -e; "
                 "export PSPDEV=/usr/local/pspdev; "
                 "export PATH=\"$PATH:$PSPDEV/bin\"; "

                 // build with psp-cmake
                 "command -v psp-cmake >/dev/null || { echo \"psp-cmake not found; PATH=$PATH\"; exit 127; }; "
                 "psp-cmake -S /source -B /build -DBUILD_PRX=1 -DENC_PRX=1 -DPSPEDIT_BUILD_GAME=1; "
                 "cmake --build /build; ";

    } else {
        script = "set -e; "
                 "export PSPDEV=/usr/local/pspdev; "
                 "export PATH=\"$PATH:$PSPDEV/bin\"; "

                 // TODO ENTT & CEREAL

                 // clone pspedit if missing
                 "if [ ! -d /source/pspedit/.git ]; then "
                 "  rm -rf /source/pspedit; "
                 "  git clone --depth 1 https://github.com/adriensalon/pspedit /source/pspedit; "
                 "fi; "

                 // build with psp-cmake
                 "command -v psp-cmake >/dev/null || { echo \"psp-cmake not found; PATH=$PATH\"; exit 127; }; "
                 "psp-cmake -S /source -B /build -DBUILD_PRX=1 -DENC_PRX=1 -DPSPEDIT_BUILD_GAME=1; "
                 "cmake --build /build; ";
    }

    std::vector<std::string> args = {
        "run", "--rm",
        "-v", src_abs.generic_string() + ":/source",
        "-v", bld_abs.generic_string() + ":/build",
        _pspdev_docker_image,
        "sh", "-lc", script
    };

    tool_result r = run_capture_combined("docker", args);
    if (r.exit_code != 0) {
        log_error("Build", "Docker build failed: " + r.stdout_text);
        return false;
    }

    log_message("Build", "Built EBOOT.pbp from Docker image" + std::string(_pspdev_docker_image));
    return true;
}

// tool_result editor_docker::run_command(const editor_docker_command& command, const tool_progress_callback& callback) const
// {
//     std::vector<std::string> _arguments;
//     _arguments.reserve(32);
//     _arguments.push_back("run");
//     if (command.is_remove_after) {
//         _arguments.push_back("--rm");
//     }
//     if (command.platform && !command.platform->empty()) {
//         _arguments.push_back("--platform");
//         _arguments.push_back(*command.platform);
//     }

//     std::vector<std::pair<std::filesystem::path, std::string>> _mounts = command.mounts;
//     if (!command.workdir_host.empty() && !command.workdir_container.empty()) {
//         bool _is_mount_existing = false;
//         for (const std::pair<std::filesystem::path, std::string>& _mount : _mounts) {
//             if (_mount.first == command.workdir_host && _mount.second == command.workdir_container) {
//                 _is_mount_existing = true;
//                 break;
//             }
//         }
//         if (!_is_mount_existing) {
//             _mounts.emplace_back(command.workdir_host, command.workdir_container);
//         }
//     }

//     for (const std::pair<std::filesystem::path, std::string>& _mount : _mounts) {
//         _arguments.push_back("-v");
//         _arguments.push_back(_mount.first.string() + ":" + _mount.second);
//     }
//     for (const std::pair<std::string, std::string>& _env : command.env) {
//         _arguments.push_back("-e");
//         _arguments.push_back(_env.first + "=" + _env.second);
//     }
//     if (!command.workdir_container.empty()) {
//         _arguments.push_back("-w");
//         _arguments.push_back(command.workdir_container);
//     }
//     _arguments.push_back(command.image);
//     for (const std::string& _command : command.command) {
//         _arguments.push_back(_command);
//     }

//     if (callback) {
//         callback({ std::nullopt, "Running docker command..." });
//     }
//     const tool_result _result = run_capture_combined(_executable, _arguments);
//     if (callback) {
//         callback({ std::nullopt, _result.exit_code == 0 ? "Docker command finished" : "Docker command failed" });
//     }

//     return _result;
// }
