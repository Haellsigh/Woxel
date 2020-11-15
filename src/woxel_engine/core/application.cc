#include "woxel_engine/core/application.hh"
#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/GLContext.h>
// glfw must be last
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace Magnum;

namespace woxel {

application::application() {
    ZoneScopedN("application::startup");

    instance_ = this;

    {
        ZoneScopedN("glfw initialization");
        if (glfwInit() == 0) {
            log::core_error("failed to initialize glfw");
            return;
        }

        glfwSetErrorCallback(
            [](int error, const char *description) { log::core_error("glfw error ({}): {}", error, description); });

        {
            ZoneScopedN("context window initialization");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
            glfwWindowHint(GLFW_RESIZABLE, true);
            glfwWindowHint(GLFW_MAXIMIZED, false);

            window_ = glfwCreateWindow(1600, 900, "Woxel", nullptr, nullptr);
            if (window_ == nullptr) {
                log::core_error("failed to create context window");
                glfwTerminate();
                return;
            }
        }

        // Create Context and Load OpenGL Functions
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(0); // Disable VSync

        {
            ZoneScopedN("callbacks initialization");
            glfwSetWindowUserPointer(window_, this);

            // GLFW callbacks
            glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
                static_cast<application *>(glfwGetWindowUserPointer(window))->close();
            });
            glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
                static_cast<application *>(glfwGetWindowUserPointer(window))
                    ->framebufferSizeCallback(window, width, height);
            });
            glfwSetKeyCallback(window_, [](GLFWwindow *win, int key, int scan, int act, int mods) {
                static_cast<application *>(glfwGetWindowUserPointer(win))->keyCallback(win, key, scan, act, mods);
            });
            glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xpos, double ypos) {
                static_cast<application *>(glfwGetWindowUserPointer(window))->mousePositionCallback(window, xpos, ypos);
            });
            glfwSetMouseButtonCallback(window_, [](GLFWwindow *window, int btn, int act, int mods) {
                static_cast<application *>(glfwGetWindowUserPointer(window))
                    ->mouseButtonCallback(window, btn, act, mods);
            });
            glfwSetScrollCallback(window_, [](GLFWwindow *window, double xoffset, double yoffset) {
                static_cast<application *>(glfwGetWindowUserPointer(window))
                    ->mousePositionCallback(window, xoffset, yoffset);
            });
        }
    }

    layer_stack_ = create_unique<layer_stack>();
    imgui_layer_ = create_unique<imgui_layer>();
    imgui_layer_->on_attach();

    {
        ZoneScopedN("magnum initialization");

        context_ = create_unique<Platform::GLContext>();
    }

    initialization_successful_ = true;
}

application::~application() {
    ZoneScoped;

    layer_stack_.reset(nullptr);

    // Shutdown context
    context_.reset(nullptr);
    // Shutdown window
    glfwTerminate();
}

void application::push_layer(std::unique_ptr<layer> layer) {
    ZoneScopedN("application::push_layer");
    layer_stack_->push_layer(std::move(layer));
}

void application::push_overlay(std::unique_ptr<layer> overlay) {
    ZoneScopedN("application::push_overlay");
    layer_stack_->push_overlay(std::move(overlay));
}

void application::close() {
    ZoneScoped;
    running_ = false;
}

auto application::get_native_window() -> void * { return window_; }

auto application::get() -> application * { return instance_; }

void application::run() {
    /**
     * Implement de "data parallel model":
     * https://www.gamasutra.com/view/feature/130247/multithreaded_game_engine_.php?page=3
     *
     * 1. Read input
     * 2. Process update in parallel, group objects by area to reduce communications
     * 3. Render
     *
     * Use entt's components:
     * 1. Transform
     * 2. ?
     *
     *
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     * https://www.gamasutra.com/blogs/MichaelKissner/20151027/257369/Writing_a_Game_Engine_from_Scratch__Part_1_Messaging.php
     *
     * use entt for messages/events:
     * https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between
     */
    ZoneScoped;

    if (!initialization_successful_) { return; }

    stopwatch_update_.start();

    // GL::defaultFramebuffer.clearColor({0.f, 0.f, 0.f});

    while (running_) {
        ZoneScopedN("single run loop");

        // events
        // 1. poll all glfw events which will call the callbacks
        // (signals and shit are already connected beforehand by each layer)
        // 2. in the callbacks functions (keyCallback, ...) fire the corresponding signals
        // 3. profit because each layer will get the good events, propagation will stop when needed, etc
        // note: we can't use entt's events for that, i think, because we can't stop propagation of signals
        {
            ZoneScopedN("Events");
            {
                ZoneScopedN("glfw poll events");
                glfwPollEvents();
            }
            FrameMarkNamed("Events");
        }

        // update
        {
            // stopwatch_update_.
            ZoneScopedN("Update");
            {
                ZoneScopedN("layers on_update");
                stopwatch_update_.lap();
                layer_stack_->each([&](auto &&layer) { layer->on_update(stopwatch_update_); });
            }
            FrameMarkNamed("Update");
        }

        // render
        {
            ZoneScopedN("Renderd");
            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
            {
                ZoneScopedN("layers on_render");
                layer_stack_->each([&](auto &&layer) { layer->on_render(); });
            }
            {

                ZoneScopedN("layers on_imgui_render");
                imgui_layer_->on_render_begin();
                {
                    layer_stack_->each([&](auto &&layer) { layer->on_imgui_render(); });
                }
                imgui_layer_->on_render_end();
            }
            {
                ZoneScopedN("glfw swap buffers");
                glfwSwapBuffers(window_);
            }
            FrameMarkNamed("Render");
        }
    }
}

void application::framebufferSizeCallback(GLFWwindow *window, int width, int height) {

    GL::defaultFramebuffer.setViewport({{}, {width, height}});
    imgui_layer_->on_framebuffer_resize({width, height});

    (void)window;
    (void)width;
    (void)height;

    // for (vector<my_class>::reverse_iterator i = my_vector.rbegin(); i != my_vector.rend(); ++i) {}
}

void application::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

void application::mousePositionCallback(GLFWwindow *window, double xpos, double ypos) {
    (void)window;
    (void)xpos;
    (void)ypos;
}

void application::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    (void)window;
    (void)button;
    (void)action;
    (void)mods;
}

void application::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    (void)window;
    (void)xoffset;
    (void)yoffset;
}

} // namespace woxel
