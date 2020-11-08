#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <spdlog/spdlog.h>

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

  if (!glfwInit()) {
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
  if (!mWindow) {
    mInitSucceeded = false;
    spdlog::error("failed to create context");
    glfwTerminate();
    return mInitSucceeded;
  }

  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  glfwSwapInterval(0);  // Disable VSync

  bgfx::renderFrame();

  bgfx::Init init;
  init.platformData.nwh = glfwGetWin32Window(mWindow);
  init.type             = bgfx::RendererType::Vulkan;
  int width, height;
  glfwGetWindowSize(mWindow, &width, &height);
  init.resolution.width  = width;
  init.resolution.height = height;
  // init.resolution.reset  = BGFX_RESET_VSYNC;

  if (!bgfx::init(init)) {
    mInitSucceeded = false;
    spdlog::error("failed to initialize bgfx");
    glfwTerminate();
    return mInitSucceeded;
  }

  constexpr bgfx::ViewId kClearView = 0;
  bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
  bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

  // GLFW callbacks
  glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* ptr, int width, int height) {
    (void)ptr;
    bgfx::reset(width, height);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
  });

  glfwSetKeyCallback(mWindow, [](GLFWwindow* ptr, int key, int scan, int act, int mods) {
    (void)ptr;
    (void)key;
    (void)scan;
    (void)act;
    (void)mods;
  });

  glfwSetCursorPosCallback(mWindow, [](GLFWwindow* ptr, double xpos, double ypos) {
    (void)ptr;
    (void)xpos;
    (void)ypos;
  });

  glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* ptr, int btn, int act, int mods) {
    (void)ptr;
    (void)btn;
    (void)act;
    (void)mods;
  });

  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();

    // clear, draw
    bgfx::touch(kClearView);
    bgfx::dbgTextClear();
    bgfx::setDebug(BGFX_DEBUG_STATS);

    bgfx::frame();
  }

  bgfx::shutdown();
  glfwTerminate();

  return 0;
}
