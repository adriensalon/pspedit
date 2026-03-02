#pragma once

#include <pspedit/core/scalar.hpp>

namespace pspedit {

struct button_state {
    bool is_pressing = false;
    bool is_pressed = false;
    bool is_releasing = false;
    bool is_released = false;
};

struct joystick_state {
    f32 x = 0.f;
    f32 y = 0.f;
};

struct input_state {
    button_state select;
    button_state l3;
    button_state r3;
    button_state start;
    button_state up;
    button_state right;
    button_state down;
    button_state left;
    button_state ltrigger;
    button_state rtrigger;
    button_state l2;
    button_state r2;
    button_state l1;
    button_state r1;
    button_state triangle;
    button_state circle;
    button_state cross;
    button_state square;
    button_state hold;
    button_state note;
    button_state volume_up;
    button_state volume_down;
    button_state wlan_up;
    button_state remote;
    joystick_state joystick;
};

}