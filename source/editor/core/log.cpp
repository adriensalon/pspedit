#include <editor/core/log.hpp>

namespace {

static std::size_t _errors_limit = 999;
static std::size_t _warnings_limit = 999;
static std::size_t _messages_limit = 999;
static std::vector<console_info> _errors = {};
static std::vector<console_info> _warnings = {};
static std::vector<console_info> _messages = {};

}

void log_error(const console_info& info)
{
    _errors.push_back(info);
}

void log_error(const std::string& category, const std::string& text)
{
    _errors.push_back({ category, text, std::nullopt, std::nullopt });
}

void log_warning(const console_info& info)
{
    _warnings.push_back(info);
}

void log_warning(const std::string& category, const std::string& text)
{
    _warnings.push_back({ category, text, std::nullopt, std::nullopt });
}

void log_message(const console_info& info)
{
    _messages.push_back(info);
}

void log_message(const std::string& category, const std::string& text)
{
    _messages.push_back({ category, text, std::nullopt, std::nullopt });
}

[[nodiscard]] std::vector<console_info>& errors()
{
    return _errors;
}

[[nodiscard]] std::vector<console_info>& warnings()
{
    return _warnings;
}

[[nodiscard]] std::vector<console_info>& messages()
{
    return _messages;
}
