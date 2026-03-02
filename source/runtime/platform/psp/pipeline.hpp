#pragma once

#include <memory>

namespace pspedit {

enum struct pipeline_start_mode {
    direct,
    call,
    send
};

struct pipeline_descriptor {
    std::size_t command_list_bytes = 256 * 1024;
    pipeline_start_mode start_mode = pipeline_start_mode::direct;
};

struct pipeline_object {
    pipeline_object(const pipeline_descriptor& descriptor = {});
    pipeline_object(const pipeline_object&) = delete;
    pipeline_object& operator=(const pipeline_object&) = delete;
    pipeline_object(pipeline_object&&) = default;
    pipeline_object& operator=(pipeline_object&&) = default;
    ~pipeline_object();

    [[nodiscard]] command_object begin_commands();
    void submit(const command_object& command);

private:
    struct pipeline_object_implementation;
    std::shared_ptr<pipeline_object_implementation> _implementation;
};
}