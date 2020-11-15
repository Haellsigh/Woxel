#pragma once

#include <woxel_engine/core/layer.hh>
#include <woxel_engine/scene/scene.hh>
#include <woxel_engine/scene/system.hh>
#include <woxel_engine/woxel_engine.hh>

#include <Magnum/GL/Buffer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector4.h>
#include <Magnum/Shaders/Phong.h>
#include <glm/glm.hpp>

struct msg1 {
    int data = 10;
};

struct msg_big {
    std::string some, more, data;
    std::array<float, 100> lots;
};

class woxel_game_layer : public woxel::layer {
  public:
    woxel_game_layer()  = default;
    ~woxel_game_layer() = default;

    void on_attach() final;
    void on_detach() final;
    void on_update(woxel::stopwatch const &s) final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    bool on_msg1(const msg1 &message);
    bool on_msg_big(const msg_big &message);

  private:
    woxel::scene scene_;
    std::size_t total_ = 0;
};
