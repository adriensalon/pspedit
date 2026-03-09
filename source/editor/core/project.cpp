#include <fstream>

#include <editor/core/docker.hpp>
#include <editor/core/log.hpp>
#include <editor/core/ppsspp.hpp>
#include <editor/core/project.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] bool _ensure_directory(const std::filesystem::path& directory)
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

    [[nodiscard]] bool _generate_cmake_file(const std::filesystem::path& source_directory)
    {
        const std::string _cmake_text = "cmake_minimum_required(VERSION 3.20) \n"
                                        "project(pspedit_game) \n"
                                        "add_subdirectory(pspedit)";

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

    [[nodiscard]] bool _install_game_executable(const std::filesystem::path& build_directory, const std::filesystem::path& install_directory)
    {
        const std::filesystem::path _eboot_build_path = build_directory / "pspeditrt" / "EBOOT.PBP";
        const std::filesystem::path _eboot_install_path = install_directory / "EBOOT.PBP";

        if (!std::filesystem::copy_file(_eboot_build_path, _eboot_install_path, std::filesystem::copy_options::overwrite_existing)) {
            log_error("Install", "Failed to install " + _eboot_install_path.string());
            return false;
        }

        log_message("Install", "Installed " + _eboot_install_path.string());
        return true;
    }

}

void open_directory(const std::filesystem::path& project_directory)
{
    if (!std::filesystem::is_directory(project_directory)) {
        log_error("Project", "Failed to open " + project_directory.string() + " is not a directory");
        return;
    }

    // TODO project file .pspeditor

    log_message("Project", "Loaded project from directory " + project_directory.string());
    current_project.reset();
    current_project = editor_project();
    current_project->directory = project_directory;
}

void save_all()
{
}

void build_and_run()
{
    if (!current_project) {
        log_error("Build", "Failed to start build no project loaded");
        return;
    }

    const std::filesystem::path _install_directory = current_project->directory / "install";
    const std::filesystem::path _cache_directory = current_project->directory / "cache";
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
    if (!_install_game_executable(_build_directory, _install_directory)) {
        return;
    }
    if (!launch_ppsspp_game(_install_directory)) {
        return;
    }
}
}
