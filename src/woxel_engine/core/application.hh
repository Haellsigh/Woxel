#pragma once

#include "woxel_engine/core/layer_stack.hh"

int main(int argc, char **argv);

struct GLFWwindow;

namespace woxel
{

class application
{
  public:
    application();
    virtual ~application();

    void push_layer(std::unique_ptr<layer> layer);
    void push_overlay(std::unique_ptr<layer> overlay);

    void close();

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
    bool running_ = true, initialization_successful_ = false;
    layer_stack layer_stack_;

    GLFWwindow *window_;
    uint16_t view_id_ = 0;
};

application *create_application();

} // namespace woxel
