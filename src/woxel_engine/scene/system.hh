#pragma once

#include "woxel_engine/core/time.hh"
#include "woxel_engine/scene/scene.hh"

#include <entt/entity/registry.hpp>

namespace woxel {

class system {
  public:
    system()          = default;
    virtual ~system() = default;

    virtual void on_attach() {}
    virtual void on_detach() {}

    virtual void on_update(stopwatch const &) {}
    virtual void on_imgui_render() {}
    virtual void on_render() {}

  protected:
    scene *scene_ = nullptr;

  private:
    friend scene;
    void set_scene(scene *s) { scene_ = s; }
};

} // namespace woxel
