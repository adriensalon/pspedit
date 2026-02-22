#include <editor/toolchain/build.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] static std::string to_container_path(const std::filesystem::path& project_dir, const std::filesystem::path& host_path, const std::string& workdir_container)
    {
        std::error_code _error;
        std::filesystem::path _relative = std::filesystem::relative(host_path, project_dir, _error);
        if (!_error && !_relative.empty() && !_relative.is_absolute()) {
            bool _has_parent = false;
            for (const std::filesystem::path& _stem : _relative) {
                if (_stem == "..") {
                    _has_parent = true;
                    break;
                }
            }
            if (!_has_parent) {
                return (std::filesystem::path(workdir_container) / _relative).generic_string();
            }
        }

        return workdir_container;
    }

    [[nodiscard]] static std::optional<std::filesystem::path> find_file_recursive(const std::filesystem::path& root, const std::string& filename)
    {
        std::error_code _error;
        if (!std::filesystem::exists(root, _error)) {
            return std::nullopt;
        }

        for (std::filesystem::recursive_directory_iterator _iterator = std::filesystem::recursive_directory_iterator(root, _error); !_error && _iterator != std::filesystem::recursive_directory_iterator(); _iterator.increment(_error)) {
            if (_error) {
                break;
            }
            if (!_iterator->is_regular_file(_error)) {
                continue;
            }
            if (_iterator->path().filename() == filename) {
                return _iterator->path();
            }
        }

        return std::nullopt;
    }

}

build_manager::build_manager(const build_configuration& configuration)
    : _configuration(configuration)
{
}

tool_result build_manager::configure_project(const docker_manager& docker, const tool_progress_callback& callback) const
{
    docker_command _command;
    _command.image = _configuration.docker_image;
    _command.platform = _configuration.platform;
    _command.workdir_host = _configuration.project_directory;
    _command.workdir_container = "/work";

    const std::string _build_directory = to_container_path(_configuration.project_directory, _configuration.build_directory, _command.workdir_container);
    _command.command = { "cmake", "-S", ".", "-B", _build_directory, "-DCMAKE_BUILD_TYPE=" + _configuration.build_type };

    if (_configuration.is_export_compile_commands) {
        _command.command.push_back("-DCMAKE_EXPORT_COMPILE_COMMANDS=ON");
    }

    return docker.run_command(_command, callback);
}

tool_result build_manager::build_project(const docker_manager& docker, const tool_progress_callback& callback) const
{
    docker_command _command;
    _command.image = _configuration.docker_image;
    _command.platform = _configuration.platform;
    _command.workdir_host = _configuration.project_directory;
    _command.workdir_container = "/work";

    const std::string _build_directory = to_container_path(_configuration.project_directory, _configuration.build_directory, _command.workdir_container);
    _command.command = { "cmake", "--build", _build_directory };

    if (_configuration.jobs_count && *_configuration.jobs_count > 0) {
        _command.command.push_back("-j");
        _command.command.push_back(std::to_string(*_configuration.jobs_count));
    }

    return docker.run_command(_command, callback);
}

}
