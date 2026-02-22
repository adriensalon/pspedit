#pragma once

#include <editor/core/tool.hpp>

namespace pspedit {

enum struct editor_action_kind {
    open_url,
    download_file,
    run_process
};

struct editor_action {
    editor_action_kind kind;
    std::string title;
    std::string description;
    std::string url;
    std::string filepath;
    std::vector<std::string> args;
    bool is_require_admin = false;
};

[[nodiscard]] tool_result execute_action(const editor_action& action, const tool_progress_callback& callback = nullptr);

}