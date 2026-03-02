#pragma once

#include <memory>
#include <optional>

#include <pspedit/core/scalar.hpp>

namespace pspedit {

enum struct buffer_kind {
    vertex,
    index
};

enum struct buffer_usage {
    static_gpu,
    dynamic_cpu_to_gpu
};

struct buffer_descriptor {
    buffer_kind kind;
    buffer_usage usage;
    u32 element_size;
    u32 element_count;
    bool in_vram = true;
    bool keep_ram_copy = false;
    std::optional<u32> vertex_type = std::nullopt;
    std::optional<u32> vertex_stride_bytes = std::nullopt;
};

struct buffer_object {
    buffer_object(vram_object& vram, const buffer_descriptor& descriptor);
    buffer_object(const buffer_object&) = delete;
    buffer_object& operator=(const buffer_object&) = delete;
    buffer_object(buffer_object&&) = default;
    buffer_object& operator=(buffer_object&&) = default;
    ~buffer_object();

    [[nodiscard]] const buffer_descriptor& descriptor() const noexcept;
    [[nodiscard]] void* map();
    void unmap();
    void upload(command_object& cmd);
    [[nodiscard]] void* ram_data() const noexcept;
    [[nodiscard]] void* vram_data() const noexcept;

private:
    struct buffer_object_implementation;
    std::shared_ptr<buffer_object_implementation> _implementation;
};
}