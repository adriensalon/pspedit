#include <sstream>

#include <editor/toolchain/docker.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] static std::string _normalize_newline(const std::string& unformatted)
    {
        std::string _formatted = trim(unformatted);
        const std::size_t _position = _formatted.find('\n');
        if (_position != std::string::npos) {
            _formatted.resize(_position);
        }
        return trim(_formatted);
    }

}

docker_manager::docker_manager(const std::string& executable)
    : _executable(executable)
{
}

docker_info docker_manager::probe_info() const
{
    docker_info _info;

    const tool_result _version_result = run_capture_combined(_executable, { "--version" });
    _info.is_executable_found = (_version_result.exit_code == 0);
    if (_info.is_executable_found) {
        _info.version = _normalize_newline(_version_result.stdout_text);
    }

    const tool_result _reachable_result = run_capture_combined(_executable, { "info" });
    _info.is_daemon_reachable = (_reachable_result.exit_code == 0);

    if (_info.is_executable_found) {
        const tool_result _server_version_result = run_capture_combined(_executable, { "version", "--format", "{{.Server.Version}}" });
        if (_server_version_result.exit_code == 0) {
            _info.server_version = _normalize_newline(_server_version_result.stdout_text);
        }
    }

    return _info;
}

docker_image_info docker_manager::inspect_image(const std::string& image) const
{
    docker_image_info _image_info;
    _image_info.name = image;

    const tool_result _inspect_result = run_capture_combined(_executable, { "image", "inspect", image, "--format", "{{.Id}}|{{.Created}}|{{.Size}}" });
    if (_inspect_result.exit_code != 0) {
        _image_info.is_present = false;
        return _image_info;
    }
    _image_info.is_present = true;

    const std::string _line = _normalize_newline(_inspect_result.stdout_text);
    std::istringstream _line_stream(_line);
    std::getline(_line_stream, _image_info.id, '|');
    std::getline(_line_stream, _image_info.created, '|');
    std::getline(_line_stream, _image_info.size, '|');
    _image_info.id = trim(_image_info.id);
    _image_info.created = trim(_image_info.created);
    _image_info.size = trim(_image_info.size);

    return _image_info;
}

tool_result docker_manager::pull_image(const std::string& image, const tool_progress_callback& callback) const
{
    if (callback) {
        callback({ std::nullopt, "Pulling docker image: " + image });
    }

    const tool_result _result = run_capture_combined(_executable, { "pull", image });
    if (callback) {
        callback({ std::nullopt, _result.exit_code == 0 ? "Docker image ready: " + image : "Failed to pull docker image: " + image });
    }

    return _result;
}

tool_result docker_manager::run_command(const docker_command& command, const tool_progress_callback& callback) const
{
    std::vector<std::string> _arguments;
    _arguments.reserve(32);
    _arguments.push_back("run");
    if (command.is_remove_after) {
        _arguments.push_back("--rm");
    }
    if (command.platform && !command.platform->empty()) {
        _arguments.push_back("--platform");
        _arguments.push_back(*command.platform);
    }

    std::vector<std::pair<std::filesystem::path, std::string>> _mounts = command.mounts;
    if (!command.workdir_host.empty() && !command.workdir_container.empty()) {
        bool _is_mount_existing = false;
        for (const std::pair<std::filesystem::path, std::string>& _mount : _mounts) {
            if (_mount.first == command.workdir_host && _mount.second == command.workdir_container) {
                _is_mount_existing = true;
                break;
            }
        }
        if (!_is_mount_existing) {
            _mounts.emplace_back(command.workdir_host, command.workdir_container);
        }
    }

    for (const std::pair<std::filesystem::path, std::string>& _mount : _mounts) {
        _arguments.push_back("-v");
        _arguments.push_back(_mount.first.string() + ":" + _mount.second);
    }
    for (const std::pair<std::string, std::string>& _env : command.env) {
        _arguments.push_back("-e");
        _arguments.push_back(_env.first + "=" + _env.second);
    }
    if (!command.workdir_container.empty()) {
        _arguments.push_back("-w");
        _arguments.push_back(command.workdir_container);
    }
    _arguments.push_back(command.image);
    for (const std::string& _command : command.command) {
        _arguments.push_back(_command);
    }

    if (callback) {
        callback({ std::nullopt, "Running docker command..." });
    }
    const tool_result _result = run_capture_combined(_executable, _arguments);
    if (callback) {
        callback({ std::nullopt, _result.exit_code == 0 ? "Docker command finished" : "Docker command failed" });
    }

    return _result;
}

}