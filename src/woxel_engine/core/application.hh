#pragma once

#include "woxel_engine/core/layer_stack.hh"
#include "woxel_engine/core/time.hh"
#include "woxel_engine/gui/imgui_layer.hh"

int main(int argc, char **argv);

struct GLFWwindow;

namespace Magnum {
namespace Platform {
class GLContext;
}
namespace GL {}
} // namespace Magnum

namespace woxel {

class application {
  public:
    application();
    virtual ~application();

    void push_layer(std::unique_ptr<layer> layer);
    void push_overlay(std::unique_ptr<layer> overlay);

    void close();

    void *get_native_window();
    static application *get();

  private:
    void run();

  private:
    friend int ::main(int, char **);

    // Callbacks
    void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

  private:
    inline static application *instance_ = nullptr;

    bool running_ = true, initialization_successful_ = false;
    stopwatch stopwatch_update_;

    GLFWwindow *window_;
    std::unique_ptr<Magnum::Platform::GLContext> context_;

    uint16_t view_id_ = 0;
    std::unique_ptr<layer_stack> layer_stack_;
    std::unique_ptr<imgui_layer> imgui_layer_;
};

application *create_application();

} // namespace woxel
