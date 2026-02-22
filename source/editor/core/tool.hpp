#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace pspedit {

struct tool_diagnostic {
    std::string summary;
    std::string detail;
};

struct tool_progress {
    std::optional<float> fraction = std::nullopt;
    std::string message;
};

using tool_progress_callback = std::function<void(const tool_progress&)>;

struct tool_result {
    int exit_code = -1;
    std::string stdout_text;
    std::string stderr_text;
};

[[nodiscard]] std::string trim(std::string untrimmed);
[[nodiscard]] std::string quote_argument(const std::string& argument);
[[nodiscard]] tool_result run_capture_combined(const std::string& executable, const std::vector<std::string>& argiments);

}