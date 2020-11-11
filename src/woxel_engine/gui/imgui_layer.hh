#pragma once

#include "woxel_engine/core/layer.hh"

namespace woxel
{

class imgui_layer : public layer
{
  public:
    imgui_layer() = default;
    ~imgui_layer() = default;

    void on_attach() override;
    void on_detach() override;

    void on_events() override;

    static void on_render_begin();
    static void on_render_end();
};

} // namespace woxel
