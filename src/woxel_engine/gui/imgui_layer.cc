#include "woxel_engine/gui/imgui_layer.hh"
#include "woxel_engine/core/application.hh"
#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"
#include "woxel_engine/gui/imgui_impl_opengl3.h"
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/GLContext.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace woxel {

void imgui_layer::on_attach() {
    ZoneScoped;

    IMGUI_CHECKVERSION();
    context_    = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 16);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(application::get()->get_native_window()), true);
    ImGui_ImplOpenGL3_Init("#version 150");

    subscribe<messages::framebuffer_size, &imgui_layer::on_framebuffer_resize>(this);

    assert(context_);
}

void imgui_layer::on_detach() {
    ZoneScoped;
    assert(context_);

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext(context_);
}

bool imgui_layer::on_framebuffer_resize(messages::framebuffer_size const &size) {
    assert(context_);

    ImGui::SetCurrentContext(context_);
    ImGui::GetIO().DisplaySize = ImVec2{static_cast<float>(size.width), static_cast<float>(size.height)};

    return true;
}

void imgui_layer::on_render_begin() {
    ZoneScoped;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_layer::on_render_end() {
    ZoneScoped;

    using namespace Magnum;

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GL::Renderer::disable(GL::Renderer::Feature::Blending);
    GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
}

} // namespace woxel
