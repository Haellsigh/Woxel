#pragma once

namespace woxel {

namespace messages {

struct framebuffer_size {
    int width, height;
};

struct key {
    int key, scancode, action, mods;
};

struct mouse_position {
    double x, y;
};

struct mouse_button {
    int button, action, mods;
};

struct mouse_scroll {
    double dx, dy;
};

} // namespace messages

} // namespace woxel
