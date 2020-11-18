#pragma once

#include "woxel_engine/core/time.hh"
#include "woxel_engine/messages/observer.hh"

#include <memory>

namespace woxel {

class layer : public observer {
  public:
    layer()          = default;
    virtual ~layer() = default;

    virtual void on_attach() {}
    virtual void on_detach() {}

    virtual void on_update(stopwatch const &) {}
    virtual void on_imgui_render() {}
    virtual void on_render() {}
};

} // namespace woxel
