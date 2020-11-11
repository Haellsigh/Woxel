#include "woxel_engine/gui/imgui_layer.hh"
#include "woxel_engine/core/application.hh"
#include "woxel_engine/debug/instrumentor.hh"

#include <imgui.h>
#include <imgui_impl_bgfx.h>
#include <imgui_impl_glfw.h>

namespace woxel
{

constexpr uint16_t imgui_view_id = 255;

void imgui_layer::on_attach()
{
    ZoneScoped;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 16);

    ImGui::StyleColorsDark();

    ImGui_Implbgfx_Init(imgui_view_id);
    ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow *>(application::get()->get_native_window()), true);
}

void imgui_layer::on_detach()
{
    ZoneScoped;

    ImGui_ImplGlfw_Shutdown();
    ImGui_Implbgfx_Shutdown();
    ImGui::DestroyContext();
}

void imgui_layer::on_events()
{
}

void imgui_layer::on_render_begin()
{
    ZoneScoped;

    ImGui_Implbgfx_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_layer::on_render_end()
{
    ZoneScoped;

    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
}

} // namespace woxel
