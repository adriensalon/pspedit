#pragma once

#include <memory>

namespace pspedit {

struct editor_window {
    editor_window(const std::string& name);
    editor_window(const editor_window& other) = delete;
    editor_window& operator=(const editor_window& other) = delete;
    editor_window(editor_window&& other) = delete;
    editor_window& operator=(editor_window&& other) = delete;
    ~editor_window();

private:
    struct editor_window_implementation;
    std::shared_ptr<editor_window_implementation> _implementation;
};

}