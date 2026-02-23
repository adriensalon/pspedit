#pragma once

#include <string>
#include <vector>

namespace pspedit {

struct tool_result {
    int exit_code = -1;
    std::string stdout_text;
    std::string stderr_text;
};

[[nodiscard]] std::string trim(std::string untrimmed);
[[nodiscard]] std::string quote_argument(const std::string& argument);
[[nodiscard]] tool_result run_capture_combined(const std::string& executable, const std::vector<std::string>& argiments);

}