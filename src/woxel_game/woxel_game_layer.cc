#include "woxel_game_layer.hh"
#include "systems/simc.hh"

#include <imgui.h>
#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>

bool f(woxel::messages::mouse_button const &b) { return b.button == 0; }

void woxel_game_layer::on_attach() {
    ZoneScoped;
    //    scene_.push_system(woxel::create_unique<metaball_renderer>());
    //    scene_.push_system(woxel::create_unique<woxel::chunk_manager>());
    scene_.push_system(woxel::create_unique<simc>());

    // subscribe<woxel::messages::mouse_button, &woxel_game_layer::on_mouse_button>(this);
    // subscribe<woxel::messages::mouse_button, &f>();
}

void woxel_game_layer::on_detach() { ZoneScoped; }

void woxel_game_layer::on_update(woxel::stopwatch const &s) {
    ZoneScopedN("dispatched message");
    scene_.on_update(s);
}

void woxel_game_layer::on_imgui_render() {
    ZoneScoped;
    ImGui::ShowDemoWindow();

    scene_.on_imgui_render();
}

void woxel_game_layer::on_render() { scene_.on_render(); }

bool woxel_game_layer::on_mouse_button(const woxel::messages::mouse_button &b) { return b.button == 0; }
