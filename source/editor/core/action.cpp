#include <editor/core/action.hpp>

namespace pspedit {

tool_result execute_action(const editor_action& action, const tool_progress_callback& callback)
{
    if (callback) {
        callback({ std::nullopt, action.title });
    }

    switch (action.kind) {
    case editor_action_kind::open_url:
#ifdef _WIN32
        return run_capture_combined("cmd", { "/c", "start", "", action.url });
#elif __APPLE__
        return run_capture_combined("open", { action.url });
#else
        return run_capture_combined("xdg-open", { action.url });
#endif

    case editor_action_kind::download_file:
#ifdef _WIN32
    {
        const std::string _powershell_download = "Invoke-WebRequest -Uri '" + action.url + "' -OutFile '" + action.filepath + "'";
        return run_capture_combined("powershell", { "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", _powershell_download });
    }
#else
        return run_capture_combined("curl", { "-L", "-o", action.filepath, action.url });
#endif

    case editor_action_kind::run_process: {
        if (action.filepath.empty()) {
            tool_result _result;
            _result.exit_code = 1;
            _result.stderr_text = "run_process action missing filepath";
            return _result;
        }
        std::vector<std::string> _arguments;
        for (const std::string& _argument : action.args) {
            _arguments.push_back(_argument);
        }
        return run_capture_combined(action.filepath, _arguments);
    }
    }

    tool_result _result;
    _result.exit_code = 1;
    _result.stderr_text = "Unknown action kind";
    return _result;
}

}