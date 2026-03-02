#pragma once

#include <memory>

namespace pspedit {

struct command_object {
    command_object(const command_object&) = delete;
    command_object& operator=(const command_object&) = delete;
    command_object(command_object&&) = default;
    command_object& operator=(command_object&&) = default;
    ~command_object();

    bool is_recording() const noexcept;
    void sync();
    void tex_sync();
    void wait_vblank();
    void set_draw_buffer(texture_format fmt, void* fb, u32 buffer_width);
    void set_depth_buffer(void* zb, u32 buffer_width);
    void set_viewport(u32 cx, u32 cy, u32 w, u32 h);
    void set_scissor(u32 x, u32 y, u32 w, u32 h);
    void clear_color(u32 rgba8888);
    void clear_depth(u16 depth);
    void clear(int mask);
    void draw_array(int prim, int vtype, int count, const void* indices, const void* vertices);

private:
    struct command_object_implementation;
    std::shared_ptr<command_object_implementation> _implementation;
};
}