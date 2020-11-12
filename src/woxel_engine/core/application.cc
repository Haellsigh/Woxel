#include "woxel_engine/core/application.hh"
#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <imgui.h>

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
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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

    {
        ZoneScopedN("bgfx initialization");
        // bgfx::renderFrame(); // set singlethreaded mode

        bgfx::Init init;
        init.platformData.nwh = glfwGetWin32Window(window_);
        init.type = bgfx::RendererType::Vulkan;
        int width, height;
        glfwGetWindowSize(window_, &width, &height);
        init.resolution.width = width;
        init.resolution.height = height;
        init.resolution.reset = BGFX_RESET_NONE;

        if (!bgfx::init(init)) {
            log::error("failed to initialize bgfx");
            glfwTerminate();
            return;
        }

        bgfx::setViewClear(view_id_, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
        bgfx::setViewRect(view_id_, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    layer_stack_ = create_unique<layer_stack>();
    imgui_layer_ = create_unique<imgui_layer>();
    layer_stack_->push_overlay(std::move(imgui_layer_));

    initialization_successful_ = true;
}

application::~application() {
    ZoneScoped;

    layer_stack_.reset(nullptr);

    // shutdown renderer
    bgfx::shutdown();

    // shutdown window
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

    while (running_) {
        ZoneScopedN("single run loop");

        {
            ZoneScopedN("glfw poll events");
            glfwPollEvents();
        }

        {
            ZoneScopedN("layers on_update");
            layer_stack_->each([](auto &&layer) { layer->on_update(); });
        }

        // get time delta

        // render
        {
            ZoneScopedN("layers on_render");
            layer_stack_->each([](auto &&layer) { layer->on_render(); });
        }

        {
            ZoneScopedN("imgui render");
            imgui_layer_->on_render_begin();

            {
                layer_stack_->each([](auto &&layer) { layer->on_imgui_render(); });
            }

            imgui_layer_->on_render_end();
        }

        {
            ZoneScopedN("glfw touch");
            bgfx::touch(view_id_);
        }

        {
            ZoneScopedN("bgfx::frame");
            bgfx::frame();
        }
        FrameMark;
    }
}

void application::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    (void)window;
    bgfx::reset(width, height);
    bgfx::setViewRect(view_id_, 0, 0, bgfx::BackbufferRatio::Equal);
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
