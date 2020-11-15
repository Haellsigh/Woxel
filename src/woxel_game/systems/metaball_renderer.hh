#pragma once

#include "woxel_game/components/metaball.hh"

#include <woxel_engine/scene/system.hh>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/Phong.h>

class metaball_renderer : public woxel::system {
  public:
    void on_attach() final;
    void on_detach() final;

    void on_update(woxel::stopwatch const &s) final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    bool use_multithreading_ = false;

    woxel::unique<Magnum::GL::Mesh> mesh_;
    woxel::unique<Magnum::GL::Buffer> buffer_;
    woxel::unique<Magnum::Shaders::Phong> shader_;

    static constexpr std::size_t k_buffer_size_bytes_ = metaball::k_max_vertices * sizeof(metaball::vertex);
};
