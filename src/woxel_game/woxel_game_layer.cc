#include "woxel_game_layer.hh"
#include "systems/metaball_renderer.hh"

#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#include <woxel_engine/scene/system.hh>

#include <imgui.h>

#include <random>

bool f(woxel::messages::mouse_button const &b) { return b.button == 0; }

void woxel_game_layer::on_attach() {
    ZoneScoped;
    scene_.push_system(woxel::create_unique<metaball_renderer>());

    subscribe<woxel::messages::mouse_button, &woxel_game_layer::on_mouse_button>(this);
    subscribe<woxel::messages::mouse_button, &f>();

    /*
    auto &&reg = scene_.get_registry();

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.5f, 1.5f);
    auto rnd = [&]() { return dist(mt); };

    for (int x = -90; x <= 90; x += 30) {
        for (int y = -50; y <= 50; y += 50) {
            auto entity        = reg.create();
            auto &m            = reg.emplace<metaball>(entity);
            m.position_        = {float(x), float(y), 100.f};
            m.rot_x_           = rnd();
            m.rot_y_           = rnd();
            m.time_multiplier_ = rnd();
        }
    }
    */
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
