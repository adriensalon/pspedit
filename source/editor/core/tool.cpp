#if defined(_WIN32)
#include <windows.h>
#endif

#include <algorithm>
#include <sstream>

#include <core/tool.hpp>

namespace {

    [[nodiscard]] static bool _is_not_space(unsigned char testchar)
    {
        return !std::isspace(testchar);
    }

}

std::string trim(std::string untrimmed)
{
    untrimmed.erase(untrimmed.begin(), std::find_if(untrimmed.begin(), untrimmed.end(), _is_not_space));
    untrimmed.erase(std::find_if(untrimmed.rbegin(), untrimmed.rend(), _is_not_space).base(), untrimmed.end());
    return untrimmed;
}

std::string quote_argument(const std::string& argument)
{
#if defined(_WIN32)
    if (argument.empty()) {
        return "\"\"";
    }
    if (argument.find_first_of(" \t\n\"&|<>^") == std::string::npos) {
        return argument;
    }

    std::string _reformatted_argument;
    _reformatted_argument.push_back('"');
    for (char _argument_char : argument) {
        if (_argument_char == '"') {
            _reformatted_argument += "\\\"";
        } else {
            _reformatted_argument.push_back(_argument_char);
        }
    }
    _reformatted_argument.push_back('"');
    return _reformatted_argument;

#else
    if (argument.empty()) {
        return "''";
    }
    if (argument.find_first_of(" \t\n'\"\\$`!") == std::string::npos) {
        return argument;
    }

    std::string _reformatted_argument = "'";
    for (char _argument_char : argument) {
        if (_argument_char == '\'') {
            _reformatted_argument += "'\\''";
        } else {
            _reformatted_argument.push_back(_argument_char);
        }
    }
    _reformatted_argument += "'";
    return _reformatted_argument;
#endif
}

tool_result run_capture_combined(const std::string& executable, const std::vector<std::string>& arguments)
{
    std::ostringstream _command_stream;
    _command_stream << quote_argument(executable);
    for (const auto& a : arguments) {
        _command_stream << " " << quote_argument(a);
    }
    _command_stream << " 2>&1";

#if defined(_WIN32)
    FILE* _file_pipe = _popen(_command_stream.str().c_str(), "r");
#else
    FILE* _file_pipe = popen(_command_stream.str().c_str(), "r");
#endif

    tool_result _result;
    if (!_file_pipe) {
        _result.exit_code = -1;
        _result.stderr_text = "Failed to spawn process.";
        return _result;
    }

    char _output_buffer[4096];
    std::string _output;
    while (std::fgets(_output_buffer, sizeof(_output_buffer), _file_pipe)) {
        _output += _output_buffer;
    }

#if defined(_WIN32)
    _result.exit_code = _pclose(_file_pipe);
#else
    int _exit_code = pclose(_file_pipe);
    if (WIFEXITED(_exit_code)) {
        _result.exit_code = WEXITSTATUS(_exit_code);
    } else {
        _result.exit_code = -1;
    }
#endif

    _result.stdout_text = _output;
    return _result;
}
