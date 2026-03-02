#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

struct console_info {
    std::string category = {};
    std::string text = {};
    std::optional<std::filesystem::path> file_path = std::nullopt;
    std::optional<std::size_t> file_line = std::nullopt;
};

void log_error(const console_info& info);
void log_error(const std::string& category, const std::string& text);
void log_warning(const console_info& info);
void log_warning(const std::string& category, const std::string& text);
void log_message(const console_info& info);
void log_message(const std::string& category, const std::string& text);
[[nodiscard]] std::vector<console_info>& errors();
[[nodiscard]] std::vector<console_info>& warnings();
[[nodiscard]] std::vector<console_info>& messages();
