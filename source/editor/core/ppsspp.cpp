#include <optional>

#include <editor/core/log.hpp>
#include <editor/core/ppsspp.hpp>
#include <editor/core/tool.hpp>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
// clang-format off
#include <windows.h>
#include <shlobj.h>
#include <knownfolders.h>
// clang-format on
#else
#include <sys/wait.h>
#endif

namespace {

static std::optional<std::filesystem::path> _ppsspp_executable = std::nullopt;

#ifdef _WIN32
static std::filesystem::path get_known_folder(REFKNOWNFOLDERID id)
{
    PWSTR w = nullptr;
    if (SHGetKnownFolderPath(id, 0, nullptr, &w) != S_OK)
        return {};
    std::filesystem::path p(w);
    CoTaskMemFree(w);
    return p;
}
#endif

[[nodiscard]] static std::optional<std::filesystem::path> _find_ppsspp_executable()
{
    std::error_code _error;

#ifdef _WIN32
    const char* _ppsspp_names[] = { "PPSSPPWindows64.exe", "PPSSPPWindows.exe", "PPSSPP.exe" };

    if (const char* env = std::getenv("PATH")) {
        std::string path(env);

        std::size_t start = 0;
        while (start <= path.size()) {
            std::size_t end = path.find(';', start);
            if (end == std::string::npos)
                end = path.size();

            std::string token = path.substr(start, end - start);
            if (!token.empty()) {
                std::filesystem::path dir(token);
                for (const char* name : _ppsspp_names) {
                    std::filesystem::path exe = dir / name;
                    if (std::filesystem::exists(exe, _error))
                        return exe;
                }
            }

            if (end == path.size())
                break;
            start = end + 1;
        }
    }

    const std::filesystem::path roots[] = {
        get_known_folder(FOLDERID_ProgramFiles),
        get_known_folder(FOLDERID_ProgramFilesX86),
    };

    auto try_path = [&](const std::filesystem::path& p) -> bool {
        std::error_code ec;
        bool ok = std::filesystem::exists(p, ec);
        // optional logging:
        // log_message("PPSSPP", "probe: " + p.string() + " ok=" + (ok?"1":"0") + " ec=" + std::to_string(ec.value()));
        return ok;
    };
    for (const auto& root : roots) {
        if (root.empty())
            continue;

        // Most common
        for (const auto& exeName : _ppsspp_names) {
            auto p = root / "PPSSPP" / exeName;
            if (try_path(p))
                return p;
        }

        // If the folder name isn't exactly "PPSSPP", brute-force one level:
        std::error_code ec;
        for (auto& e : std::filesystem::directory_iterator(root, ec)) {
            if (!e.is_directory())
                continue;
            auto dir = e.path().filename().string();
            if (dir.find("PPSSPP") == std::string::npos)
                continue;
            for (const auto& exeName : _ppsspp_names) {
                auto p = e.path() / exeName;
                if (try_path(p))
                    return p;
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

[[nodiscard]] static bool _is_ppsspp_found()
{
    if (!_ppsspp_executable) {
        _ppsspp_executable = _find_ppsspp_executable();
        if (!_ppsspp_executable) {
            log_error("Debug", "PPSSPP not found please install from ...");
            return false;
        }
    }
    log_message("Debug", "Found PPSSPP at " + _ppsspp_executable->string());
    return true;
}

}

bool launch_ppsspp_game(const std::filesystem::path& install_directory, const std::vector<std::string>& arguments)
{
    if (!_is_ppsspp_found()) {
        return false;
    }

    std::vector<std::string> _arguments = {};
    _arguments.push_back(install_directory.string());
    for (const std::string& _argument : arguments) {
        _arguments.push_back(_argument);
    }
    const tool_result _result = run_capture_combined(_ppsspp_executable.value().string(), _arguments);
    return true;
}
