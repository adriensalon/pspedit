#include <editor/toolchain/ppsspp.hpp>

#ifndef _WIN32
#include <sys/wait.h>
#endif

namespace pspedit {
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
                    if (std::filesystem::exists(_ppsspp_path, _error))
                        return p;
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

        const std::filesystem::path roots[] = { "/usr/bin", "/usr/local/bin", "/snap/bin" };
        for (const auto& root : roots) {
            for (const char* exe : _ppsspp_names) {
                std::filesystem::path _ppsspp_path = root / exe;
                if (std::filesystem::exists(_ppsspp_path, _error)) {
                    return _ppsspp_path;
                }
            }
        }
#endif

        return std::nullopt;
    }

}

ppsspp_manager::ppsspp_manager(const ppsspp_configuration& configuration)
    : _configuration(configuration)
{
}

ppsspp_info ppsspp_manager::probe_info() const
{
    const std::optional<std::filesystem::path> _executable = _find_ppsspp_executable();

    ppsspp_info _info;
    _info.is_executable_found = _executable.has_value();
    _info.executable = _executable ? _executable.value() : "";

    return _info;
}

tool_result ppsspp_manager::launch_game(const std::filesystem::path& game_directory, const std::vector<std::string>& arguments) const
{
    const ppsspp_info _info = probe_info();
    if (!_info.is_executable_found) {
        tool_result _result;
        _result.exit_code = 1;
        _result.stderr_text = "PPSSPP executable not found";
        return _result;
    }

    std::vector<std::string> _arguments = arguments;
    _arguments.push_back(game_directory.string());

    return run_capture_combined(_info.executable.string(), _arguments);
}

}