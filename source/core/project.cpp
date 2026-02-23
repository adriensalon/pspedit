#include <core/log.hpp>
#include <core/project.hpp>
#include <core/docker.hpp>

namespace {

[[nodiscard]] bool _ensure_directory(const std::filesystem::path directory)
{
    if (!std::filesystem::is_directory(directory)) {
        if (!std::filesystem::create_directory(directory)) {
            log_error("Project", "Failed to create directory " + directory.string());
            return false;
        }
        log_message("Project", "Created directory " + directory.string());
    }
    return true;
}

static std::optional<std::filesystem::path> _project_directory = std::nullopt;

}

void open_directory(const std::filesystem::path& project_directory)
{
    if (!std::filesystem::is_directory(project_directory)) {
        log_error("Project", "Failed to open " + project_directory.string() + " is not a directory");
        return;
    }

    // TODO project file .pspeditor

    log_message("Project", "Loaded project from directory " + project_directory.string());
    _project_directory = project_directory;
}

std::optional<std::filesystem::path> project_directory()
{
    return _project_directory;
}

void save_all()
{
}

void build_and_run()
{
    if (!_project_directory) {
        log_error("Build", "Failed to start build no project loaded");
        return;
    }

    const std::filesystem::path _install_directory = _project_directory.value() / "install";
    const std::filesystem::path _cache_directory = _project_directory.value() / "cache";
    const std::filesystem::path _containers_directory = _cache_directory / "containers";
    const std::filesystem::path _generated_directory = _cache_directory / "generated";
    const std::filesystem::path _build_directory = _cache_directory / "build";
    if (!_ensure_directory(_install_directory)
        || !_ensure_directory(_cache_directory)
        || !_ensure_directory(_containers_directory)
        || !_ensure_directory(_generated_directory)
        || !_ensure_directory(_build_directory)) {
        return;
    }

    if (!ensure_docker_ready()) {
        return;
    }
}
