#pragma once

#include <woxel_engine/core/layer.hh>
#include <woxel_engine/scene/scene.hh>
#include <woxel_engine/scene/system.hh>
#include <woxel_engine/woxel_engine.hh>

#include <glm/glm.hpp>

struct transform {
    glm::vec3 translation = {0.f, 0.f, 0.f};
    glm::vec3 rotation    = {0.f, 0.f, 0.f};
    glm::vec3 scale       = {1.f, 1.f, 1.f};
};

struct renderable_2d {
    glm::vec4 color = {1.f, 1.f, 1.f, 1.f};
};

class renderer2d_system : public woxel::system {
    void on_attach() final;
    void on_detach() final;

    void on_update() final;
    void on_render() final;

  private:
};

class woxel_game_layer : public woxel::layer {
  public:
    woxel_game_layer()  = default;
    ~woxel_game_layer() = default;

    void on_attach() final;
    void on_detach() final;
    void on_update() final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    woxel::scene scene_;
};
