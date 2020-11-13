#pragma once

#include "woxel_engine/core/layer.hh"

#include <array>

struct ImGuiContext;

namespace woxel {

class imgui_layer : public layer {
  public:
    imgui_layer()  = default;
    ~imgui_layer() = default;

    void on_attach() override;
    void on_detach() override;

    void on_framebuffer_resize(std::array<int, 2> const &size);
    static void on_render_begin();
    static void on_render_end();

  private:
    ImGuiContext *context_ = nullptr;
};

} // namespace woxel
