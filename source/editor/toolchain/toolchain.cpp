#include <editor/toolchain/toolchain.hpp>

#ifndef _WIN32
#include <sys/wait.h>
#endif

namespace pspedit {

toolchain_manager::toolchain_manager(const toolchain_configuration& configuration)
    : docker(
          configuration.docker_executable)
    , build(
          build_configuration {
              configuration.pspdev_image,
              std::nullopt,
              configuration.workspace_directory,
              configuration.workspace_directory / "build",
              configuration.memory_stick_directory,
          })
    , ppsspp(
          ppsspp_configuration {
              configuration.workspace_directory / "install",
              configuration.memory_stick_directory })
    , _configuration(configuration)
{
}

std::vector<editor_action> toolchain_manager::detect_actions(std::vector<tool_diagnostic>& diagnostics, const tool_progress_callback& callback) const
{
    std::vector<editor_action> _actions;
    const docker_info _docker_info = docker.probe_info();

    if (!_docker_info.is_executable_found) {
        diagnostics.push_back({ "Docker not found", "Docker executable not available" });

        editor_action _action;
        _action.kind = editor_action_kind::open_url;
        _action.title = "Install Docker";
        _action.description = "Docker must be installed on the host computer";
        _action.url = "https://www.docker.com/products/docker-desktop/";
        _actions.push_back(std::move(_action));
        return _actions;
    }

    if (!_docker_info.is_daemon_reachable) {
        diagnostics.push_back({ "Docker daemon not reachable", "Docker is installed but the daemon is not reachable, start Docker Engine" });

    } else {
        const docker_image_info _image_info = docker.inspect_image(_configuration.pspdev_image);
        if (!_image_info.is_present) {
            diagnostics.push_back({ "PSPDEV image missing", _configuration.pspdev_image });

            editor_action _action;
            _action.kind = editor_action_kind::run_process;
            _action.title = "Pull PSPDEV Docker image";
            _action.description = "Download the PSPDEV toolchain image";
            _action.filepath = _configuration.docker_executable;
            _action.args = { "pull", _configuration.pspdev_image };
            _actions.push_back(std::move(_action));
        }
    }

    const ppsspp_info _ppsspp_info = ppsspp.probe_info();
    if (!_ppsspp_info.is_executable_found) {
        diagnostics.push_back({ "PPSSPP missing", "PPSSPP executable not found in install directory" });

        editor_action _action;
        _action.kind = editor_action_kind::open_url;
        _action.title = "Install PPSSPP";
        _action.description = "Download and install PPSSPP (emulator) into the configured install directory";
        _action.url = "https://www.ppsspp.org/download/";
        _actions.push_back(std::move(_action));
    }

    if (_actions.empty()) {
        // ADD CONFIGURE AND BUILD ACTIONS
    }

    if (callback) {
        callback({ 1.f, "Toolchain checks complete" });
    }

    return _actions;
}

}