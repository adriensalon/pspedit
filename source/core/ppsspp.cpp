#include <optional>

#include <core/ppsspp.hpp>
#include <core/tool.hpp>
#include <core/log.hpp>

#ifndef _WIN32
#include <sys/wait.h>
#endif

namespace {

    [[nodiscard]] static std::optional<std::filesystem::path> _find_ppsspp_executable()
    {
        std::error_code _error;

#ifdef _WIN32
        const char* _ppsspp_names[] = { "PPSSPPWindows64.exe", "PPSSPPWindows.exe", "PPSSPP.exe" };

        if (const char* _env_path_cstr = std::getenv("PATH")) {
            std::string _env_path(_env_path_cstr);
            std::size_t _position = 0;
            while ((_position = _env_path.find(';')) != std::string::npos) {
                const std::filesystem::path _env_directory = _env_path.substr(0, _position);
                _env_path.erase(0, _position + 1);
                for (const char* _ppsspp_name : _ppsspp_names) {
                    std::filesystem::path _ppsspp_path = _env_directory / _ppsspp_name;
                    if (std::filesystem::exists(_ppsspp_path, _error)) {
                        return _ppsspp_path;
                    }
                }
            }
        }

        const char* _program_files = std::getenv("ProgramFiles");
        const char* _program_files_x86 = std::getenv("ProgramFiles(x86)");
        const std::filesystem::path _win32_roots[] = {
            _program_files ? std::filesystem::path(_program_files) : std::filesystem::path(),
            _program_files_x86 ? std::filesystem::path(_program_files_x86) : std::filesystem::path()
        };
        for (const std::filesystem::path& _win32_root : _win32_roots) {
            if (_win32_root.empty()) {
                continue;
            }
            const std::filesystem::path _win32_path = _win32_root / "PPSSPP";
            for (const char* _ppsspp_name : _ppsspp_names) {
                const std::filesystem::path _ppsspp_path = _win32_path / _ppsspp_name;
                if (std::filesystem::exists(_ppsspp_path, _error)) {
                    return _ppsspp_path;
                }
            }
        }

#elif __APPLE__
        const char* _ppsspp_names[] = { "PPSSPP" };

        if (const char* _env_path_cstr = std::getenv("PATH")) {
            std::string _env_path(_env_path_cstr);
            std::size_t _position = 0;
            while ((_position = _env_path.find(':')) != std::string::npos) {
                const std::filesystem::path _env_directory = _env_path.substr(0, _position);
                _env_path.erase(0, _position + 1);

                for (const char* _ppsspp_name : _ppsspp_names) {
                    const std::filesystem::path _ppsspp_path = _env_directory / _ppsspp_name;
                    if (std::filesystem::exists(_ppsspp_path, _error)) {
                        return _ppsspp_path;
                    }
                }
            }
        }

        const std::filesystem::path _app_path = "/Applications/PPSSPP.app/Contents/MacOS/PPSSPP";
        if (std::filesystem::exists(_app_path, _error)) {
            return _app_path;
        }

#else
        const char* _ppsspp_names[] = { "ppsspp", "PPSSPP" };

        if (const char* _env_path_cstr = std::getenv("PATH")) {
            std::string _env_path(_env_path_cstr);
            std::size_t _position = 0;
            while ((_position = _env_path.find(':')) != std::string::npos) {
                const std::filesystem::path _env_directory = _env_path.substr(0, _position);
                _env_path.erase(0, _position + 1);
                for (const char* _ppsspp_name : _ppsspp_names) {
                    const std::filesystem::path _ppsspp_path = _env_directory / _ppsspp_name;
                    if (std::filesystem::exists(_ppsspp_path, _error)) {
                        return _ppsspp_path;
                    }
                }
            }
        }

        const std::filesystem::path _root_paths[] = { "/usr/bin", "/usr/local/bin", "/snap/bin" };
        for (const std::filesystem::path& _root_path : _root_paths) {
            for (const char* exe : _ppsspp_names) {
                std::filesystem::path _ppsspp_path = _root_path / exe;
                if (std::filesystem::exists(_ppsspp_path, _error)) {
                    return _ppsspp_path;
                }
            }
        }
#endif

        return std::nullopt;
    }

    static std::optional<std::filesystem::path> _ppsspp_executable = std::nullopt;
}

bool is_ppsspp_found()
{
    if (!_ppsspp_executable) {
        _ppsspp_executable = _find_ppsspp_executable();
    }
    return _ppsspp_executable.has_value();
}

void launch_ppsspp_game(const std::filesystem::path& game_directory, const std::vector<std::string>& arguments)
{
    if (!_ppsspp_executable) {
        log_error("Run", "PPSSPP executable was not found on the host machine");
        return;
    }

    std::vector<std::string> _arguments = {};
    _arguments.push_back(game_directory.string());
    for (const std::string& _argument : arguments) {
        _arguments.push_back(_argument);
    }
    const tool_result _result = run_capture_combined(_ppsspp_executable.value().string(), _arguments);
}
