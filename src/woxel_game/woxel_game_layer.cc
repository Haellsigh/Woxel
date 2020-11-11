#include "woxel_game_layer.hh"

#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>

#include <imgui.h>

void woxel_game_layer::on_attach()
{
    ZoneScoped;
    woxel::log::trace("woxel_game_layer::on_attach");
}

void woxel_game_layer::on_detach()
{
    ZoneScoped;
    woxel::log::trace("woxel_game_layer::on_detach");
}

void woxel_game_layer::on_events()
{
}

void woxel_game_layer::on_update()
{
}

void woxel_game_layer::on_imgui_render()
{
    ZoneScoped;
    ImGui::ShowDemoWindow();
}

void woxel_game_layer::on_render()
{
}
