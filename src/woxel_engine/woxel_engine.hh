#pragma once

#include "woxel_engine/core/application.hh"
#include "woxel_engine/core/base.hh"

/*
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui.h>
#include <imgui_impl_bgfx.h>
#include <imgui_impl_glfw.h>
#include <spdlog/spdlog.h>

#include <Tracy.hpp>

// GLFW must be included after
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

int main()
{
    bool mInitSucceeded = true;

    if (!glfwInit())
    {
        mInitSucceeded = false;
        spdlog::error("failed to initialize glfw");
        return mInitSucceeded;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_MAXIMIZED, false);

    auto mWindow = glfwCreateWindow(1600, 900, "Woxel", nullptr, nullptr);
    if (!mWindow)
    {
        mInitSucceeded = false;
        spdlog::error("failed to create context");
        glfwTerminate();
        return mInitSucceeded;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(0); // Disable VSync

    bgfx::renderFrame(); // set singlethreaded mode

    bgfx::Init init;
    init.platformData.nwh = glfwGetWin32Window(mWindow);
    init.type = bgfx::RendererType::Vulkan;
    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);
    init.resolution.width = width;
    init.resolution.height = height;
    // init.resolution.reset  = BGFX_RESET_VSYNC;

    if (!bgfx::init(init))
    {
        mInitSucceeded = false;
        spdlog::error("failed to initialize bgfx");
        glfwTerminate();
        return mInitSucceeded;
    }

    constexpr bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    // GLFW callbacks
    glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow *ptr, int width, int height) {
        (void)ptr;
        bgfx::reset(width, height);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    });

    glfwSetKeyCallback(mWindow, [](GLFWwindow *ptr, int key, int scan, int act, int mods) {
        (void)ptr;
        (void)key;
        (void)scan;
        (void)act;
        (void)mods;
    });

    glfwSetCursorPosCallback(mWindow, [](GLFWwindow *ptr, double xpos, double ypos) {
        (void)ptr;
        (void)xpos;
        (void)ypos;
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *ptr, int btn, int act, int mods) {
        (void)ptr;
        (void)btn;
        (void)act;
        (void)mods;
    });

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 16);
    ImGui::StyleColorsDark();

    ImGui_Implbgfx_Init(255);
    ImGui_ImplGlfw_InitForVulkan(mWindow, true);

    while (!glfwWindowShouldClose(mWindow))
    {
        FrameMark;
        glfwPollEvents();

        ImGui_Implbgfx_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

        // clear, draw
        bgfx::touch(kClearView);
        bgfx::dbgTextClear();
        bgfx::setDebug(BGFX_DEBUG_STATS);

        bgfx::frame();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_Implbgfx_Shutdown();
    ImGui::DestroyContext();
    bgfx::shutdown();
    glfwTerminate();

    return 0;
}
*/
