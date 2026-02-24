#include <fstream>

#include <core/docker.hpp>
#include <core/log.hpp>
#include <core/project.hpp>

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

[[nodiscard]] bool _generate_cmake_file(const std::filesystem::path source_directory)
{
    const std::string _cmake_text = "cmake_minimum_required(VERSION 3.20) \n"
                                    "project(pspengine_game) \n"
                                    "add_subdirectory(pspeditrt)";

    const std::filesystem::path _cmake_path = source_directory / "CMakeLists.txt";
    std::ofstream _cmake_stream(_cmake_path, std::ios::binary);
    if (!_cmake_stream.is_open()) {
        log_error("Project", "Failed to open " + _cmake_path.string());
        return false;
    }
    _cmake_stream << _cmake_text;
    if (!_cmake_stream.good()) {
        log_error("Project", "Failed to write " + _cmake_path.string());
        return false;
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
    const std::filesystem::path _source_directory = _cache_directory / "source";
    const std::filesystem::path _build_directory = _cache_directory / "build";
    
    if (!_ensure_directory(_install_directory)) {
        return;
    }
    if (!_ensure_directory(_cache_directory)) {
        return;
    }
    if (!_ensure_directory(_containers_directory)) {
        return;
    }
    if (!_ensure_directory(_source_directory)) {
        return;
    }
    if (!_generate_cmake_file(_source_directory)) {
        return;
    }
    if (!_ensure_directory(_build_directory)) {
        return;
    }
    if (!docker_build_project(_source_directory, _build_directory)) {
        return;
    }

    // install 
}
