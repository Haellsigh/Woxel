#pragma once

#include "woxel_engine/scene/system.hh"
#include "woxel_engine/scene/world/chunk_container.hh"

namespace woxel {

class chunk_manager : public system {
  public:
    void on_attach() final;
    void on_detach() final;

    void on_update(const stopwatch &) final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    chunk_container chunk_container_;
};

} // namespace woxel
