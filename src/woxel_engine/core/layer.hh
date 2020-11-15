#pragma once

#include "woxel_engine/core/time.hh"
#include "woxel_engine/messages/message.hh"

#include <memory>

namespace woxel {

class layer {
  public:
    layer()          = default;
    virtual ~layer() = default;

    template <typename message_type> bool on_message(const message_type &message) {
        return message_handler_.send(message);
    }

    virtual void on_attach() {}
    virtual void on_detach() {}

    virtual void on_update(stopwatch const &) {}
    virtual void on_imgui_render() {}
    virtual void on_render() {}

  protected:
    message_handler message_handler_;
};

} // namespace woxel
