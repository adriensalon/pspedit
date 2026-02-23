#include <core/log.hpp>
#include <core/project.hpp>

namespace {

bool _ensure_directory(const std::filesystem::path directory)
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

    const std::filesystem::path _install_directory = project_directory / "install";
    const std::filesystem::path _cache_directory = project_directory / "cache";
    const std::filesystem::path _containers_directory = _cache_directory / "containers";
    const std::filesystem::path _build_directory = _cache_directory / "build";
    if (!_ensure_directory(_install_directory)
        || !_ensure_directory(_cache_directory)
        || !_ensure_directory(_containers_directory)
        || !_ensure_directory(_build_directory)) {
        return;
    }

    // TODO project file .pspeditor

    _project_directory = project_directory;
}

std::optional<std::filesystem::path> project_directory()
{
    return _project_directory;
}

void save_all()
{
}
