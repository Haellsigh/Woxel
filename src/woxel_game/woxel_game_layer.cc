#include "woxel_game_layer.hh"
#include "systems/metaball_renderer.hh"

#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#include <woxel_engine/scene/system.hh>

#include <imgui.h>

#include <random>

void woxel_game_layer::on_attach() {
    ZoneScoped;
    scene_.push_system(woxel::create_unique<metaball_renderer>());

    woxel::log::debug("listen<msg1>()");
    message_handler_.listen<msg1, &woxel_game_layer::on_msg1>(this);
    woxel::log::debug("listen<msg_big>()");
    message_handler_.listen<msg_big, &woxel_game_layer::on_msg_big>(this);

    std::size_t n = 1000000;
    woxel::stopwatch s;
    msg_big big;
    big.some = "";
    s.start();
    for (std::size_t i = 0; i < n; ++i) {
        message_handler_.send(big);
    }
    s.lap();
    woxel::log::debug("{} iterations took {} ms ({} ns each)", n, s.last_lap().as_milliseconds(),
                      s.last_lap().as_nanoseconds() / n);

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
    message_handler_.send(msg1{});
    scene_.on_update(s);
}

void woxel_game_layer::on_imgui_render() {
    ZoneScoped;
    ImGui::ShowDemoWindow();

    scene_.on_imgui_render();
}

void woxel_game_layer::on_render() { scene_.on_render(); }

bool woxel_game_layer::on_msg1(const msg1 &message) {
    total_ += message.data;
    // ZoneScopedN("received message");
    // woxel::log::trace("message: {}", message.data);
    return true;
}

bool woxel_game_layer::on_msg_big(const msg_big &message) {
    total_ += message.some.size();
    return true;
}
