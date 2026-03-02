#pragma once

#include <chrono>
#include <future>

#include <pspedit/core/input.hpp>

namespace pspedit {

struct create_context {

};

struct update_context {
    input_state input;
    std::chrono::milliseconds delta_time;

    const std::vector<std::string>& local_gamesaves();
    std::future<void> load_gamesave(const std::string& name);
    std::future<void> save_gamesave(const std::string& name);

    shared_scene current_scene();


    shared_image create_image(const image_object& format);
    shared_material create_material(const psp_shader_descriptor& descriptor);
    shared_transform create_transform();

    runtime_image* resolve_image(const shared_image image);
    runtime_material* resolve_material(const shared_material material);
    runtime_transform* resolve_transform(const shared_transform transform);
    template <typename Script> 
    runtime_script<Script>* resolve_script(const shared_script<Script> script);
    runtime_scene* resolve_scene(const shared_scene scene);
};


struct destroy_context {

};

}