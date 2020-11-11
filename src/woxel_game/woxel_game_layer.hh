#pragma once

#include <woxel_engine/core/layer.hh>

class woxel_game_layer : public woxel::layer
{
  public:
    woxel_game_layer() = default;
    ~woxel_game_layer() = default;

    void on_attach() final;
    void on_detach() final;
    void on_events() final;
    void on_update() final;
    void on_imgui_render() final;
    void on_render() final;
};
