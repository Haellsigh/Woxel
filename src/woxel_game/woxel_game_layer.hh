#pragma once

#include <woxel_engine/core/layer.hh>
#include <woxel_engine/scene/scene.hh>
#include <woxel_engine/scene/system.hh>
#include <woxel_engine/woxel_engine.hh>

#include <bgfx/bgfx.h>
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)

struct transform {
    glm::vec3 translation = {0.f, 0.f, 0.f};
    glm::vec3 rotation    = {0.f, 0.f, 0.f};
    glm::vec3 scale       = {0.f, 0.f, 0.f};
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
    bgfx::VertexBufferHandle vbh_;
    bgfx::IndexBufferHandle ibh_;
    bgfx::ShaderHandle vsh_, fsh_;
    bgfx::ProgramHandle program_;
};

class woxel_game_layer : public woxel::layer {
  public:
    woxel_game_layer()  = default;
    ~woxel_game_layer() = default;

    void on_attach() final;
    void on_detach() final;
    void on_events() final;
    void on_update() final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    woxel::scene scene_;
};
