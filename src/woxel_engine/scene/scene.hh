#pragma once

#include "woxel_engine/core/base.hh"
#include "woxel_engine/core/time.hh"

#include <entt/entt.hpp>

#include <vector>

namespace woxel {

class system;

class scene {
  public:
    scene() = default;
    ~scene();

    void push_system(unique<system> system);

    entt::registry &get_registry();

    void on_update(stopwatch const &s);
    void on_imgui_render();
    void on_render();

  private:
    entt::registry registry_;
    std::vector<unique<system>> systems_;
};

} // namespace woxel
