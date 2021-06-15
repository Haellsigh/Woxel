#pragma once

#include <woxel_engine/core/layer.hh>
#include <woxel_engine/scene/scene.hh>
#include <woxel_engine/scene/system.hh>
#include <woxel_engine/woxel_engine.hh>

class woxel_game_layer : public woxel::layer {
  public:
    woxel_game_layer()  = default;
    ~woxel_game_layer() = default;

    void on_attach() final;
    void on_detach() final;

    void on_update(woxel::stopwatch const &s) final;
    void on_imgui_render() final;
    void on_render() final;

    bool on_mouse_button(woxel::messages::mouse_button const &b);

  private:
  private:
    woxel::scene scene_;
    std::size_t total_ = 0;
};
