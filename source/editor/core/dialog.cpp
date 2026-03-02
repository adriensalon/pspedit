#include <nfd.h>

#include <core/dialog.hpp>

namespace {

[[nodiscard]] static std::string _build_filters(const std::vector<std::pair<std::string, std::string>>& filters)
{
    std::string _filters;
    bool _is_first = true;
    for (const std::pair<const std::string, std::string>& _filter : filters) {
        const std::string& _pattern = _filter.second;
        std::size_t _start = 0;
        while (_start < _pattern.size()) {
            
            std::size_t _end = _pattern.find(';', _start);
            if (_end == std::string::npos) {
                _end = _pattern.size();
            }

            std::string _token = _pattern.substr(_start, _end - _start);
            const std::size_t _trim_front = _token.find_first_not_of(" \t");
            if (_trim_front != std::string::npos) {
                _token.erase(0, _trim_front);
            }

            const std::size_t trim_back = _token.find_last_not_of(" \t");
            if (trim_back != std::string::npos) {
                _token.erase(trim_back + 1);
            }

            if (_token.rfind("*.", 0) == 0) {
                _token.erase(0, 2);
            } else if (!_token.empty() && _token[0] == '.') {
                _token.erase(0, 1);
            }

            if (!_token.empty()) {
                if (!_is_first) {
                    _filters += ",";
                }
                _filters += _token;
                _is_first = false;
            }
            _start = _end + 1;
        }
    }

    return _filters;
}

[[nodiscard]] static std::string _to_utf8_path(const std::filesystem::path& path)
{
#if defined(_WIN32)
    return path.generic_u8string();
#else
    return path.string();
#endif
}

[[nodiscard]] static std::filesystem::path _from_utf8_path(const nfdchar_t* path)
{
#if defined(_WIN32)
    return std::filesystem::u8path(path);
#else
    return std::filesystem::path(path);
#endif
}

}

std::optional<std::filesystem::path> open_file_dialog(
    const std::vector<std::pair<std::string, std::string>>& filters,
    const std::filesystem::path& default_path)
{
    const std::string _filters = _build_filters(filters);
    const nfdchar_t* _filters_cstr = _filters.empty() ? nullptr : _filters.c_str();
    const std::string _default_utf8 = default_path.empty() ? std::string {} : _to_utf8_path(default_path);
    const nfdchar_t* _default_cstr = _default_utf8.empty() ? nullptr : _default_utf8.c_str();
    nfdchar_t* _filepath_cstr = nullptr;
    
    if (NFD_OpenDialog(_filters_cstr, _default_cstr, &_filepath_cstr) != NFD_OKAY) {
        return std::nullopt;
    }
    const std::filesystem::path _path = _from_utf8_path(_filepath_cstr);
    free(_filepath_cstr);
    
    return _path;
}

std::optional<std::filesystem::path> save_file_dialog(
    const std::vector<std::pair<std::string, std::string>>& filters,
    const std::filesystem::path& default_path)
{
    const std::string _filters = _build_filters(filters);
    const nfdchar_t* _filters_cstr = _filters.empty() ? nullptr : _filters.c_str();
    const std::string _default_utf8 = default_path.empty() ? std::string {} : _to_utf8_path(default_path);
    const nfdchar_t* _default_cstr = _default_utf8.empty() ? nullptr : _default_utf8.c_str();
    nfdchar_t* _filepath_cstr = nullptr;
    
    if (NFD_SaveDialog(_filters_cstr, _default_cstr, &_filepath_cstr) != NFD_OKAY) {
        return std::nullopt;
    }
    const std::filesystem::path _path = _from_utf8_path(_filepath_cstr);
    std::free(_filepath_cstr);
    
    return _path;
}

std::optional<std::filesystem::path> pick_directory_dialog(
    const std::filesystem::path& default_path)
{
    const std::string _default_utf8 = default_path.empty() ? std::string {} : _to_utf8_path(default_path);
    const nfdchar_t* _default_cstr = _default_utf8.empty() ? nullptr : _default_utf8.c_str();
    nfdchar_t* _dirpath_cstr = nullptr;
    
    if (NFD_PickFolder(_default_cstr, &_dirpath_cstr) != NFD_OKAY) {
        return std::nullopt;
    }
    const std::filesystem::path _path = _from_utf8_path(_dirpath_cstr);
    std::free(_dirpath_cstr);
    
    return _path;
}