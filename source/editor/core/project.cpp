#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/string.hpp>

#include <common/asset/content.hpp>
#include <common/asset/package.hpp>
#include <common/asset/scene.hpp>
#include <editor/core/docker.hpp>
#include <editor/core/log.hpp>
#include <editor/core/ppsspp.hpp>
#include <editor/core/project.hpp>

namespace cereal {

template <class Archive>
void save(Archive& archive, const std::filesystem::path& value)
{
    archive(value.string());
}

template <class Archive>
void load(Archive& archive, std::filesystem::path& value)
{
    std::string temp;
    archive(temp);
    value = temp;
}

}

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

    void _ensure_loaded_content_asset(editor_project* project)
    {
        const std::filesystem::path _content_asset_path = project->directory / "install" / "assets" / "content.bin";
        
		if (!std::filesystem::exists(_content_asset_path)) {
            const std::filesystem::path _main_package_asset_path = project->directory / "install" / "assets" / "mainpackage.bin";
            const std::filesystem::path _main_scene_asset_path = project->directory / "install" / "assets" / "mainscene.bin";
			
			const package_id _main_package_id = project->ids.new_package_id();
			const scene_id _main_scene_id = project->ids.new_scene_id();
			project_asset<package_asset>& _main_package = project->packages[_main_package_id];
            project_asset<scene_asset>& _main_scene = project->scenes[_main_scene_id];
            _main_package.editor_name = "mainpackage";
            _main_package.asset.scenes.emplace(_main_scene_id, _main_scene_asset_path);
            _main_scene.editor_name = "mainscene";
			save_asset(_main_package_asset_path, _main_package.asset);
			save_asset(_main_scene_asset_path, _main_scene.asset);
            
			project->content = {};
            project->content.main_package = _main_package_id;
            project->content.packages.emplace(project->content.main_package, _main_package_asset_path);
            project->content.main_scene = _main_scene_id;
			save_asset(_content_asset_path, project->content);

        } else {
			load_asset(_content_asset_path, project->content);
			// load_asset(project->content)

		}
    }

}

void open_directory(const std::filesystem::path& project_directory)
{
    if (!std::filesystem::is_directory(project_directory)) {
        log_error("Project", "Failed to open " + project_directory.string() + " is not a directory");
        return;
    }

    current_project.reset();
    current_project = editor_project();
    current_project->directory = project_directory;

    const std::filesystem::path _settings_path = project_directory / "settings.psp.json";
    if (!std::filesystem::exists(_settings_path)) {
        std::ofstream _fstream(_settings_path);
        cereal::JSONOutputArchive _archive(_fstream);
        _archive(cereal::make_nvp("name", current_project->name));
        // _archive(cereal::make_nvp("cover_image", current_project->cover_image));
        _archive(cereal::make_nvp("heap_memory", current_project->heap_memory));
        _archive(cereal::make_nvp("custom_pspedit_dir", current_project->custom_pspedit_dir));
    } else {
        std::ifstream _fstream(_settings_path);
        cereal::JSONInputArchive _archive(_fstream);
        _archive(current_project->name);
        // _archive(current_project->cover_image);
        _archive(current_project->heap_memory);
        _archive(current_project->custom_pspedit_dir);
    }

    log_message("Project", "Loaded project from directory " + project_directory.string());
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
    if (!docker_build_project(_source_directory, _build_directory, current_project->custom_pspedit_dir)) {
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
