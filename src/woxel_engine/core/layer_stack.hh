#pragma once

#include "woxel_engine/core/layer.hh"

#include <memory>
#include <vector>

namespace woxel
{

class layer_stack
{
  public:
    using layer_ptr = std::unique_ptr<layer>;

    layer_stack() = default;
    ~layer_stack();

    void push_layer(layer_ptr layer);
    void push_overlay(layer_ptr overlay);

    void pop_layer(layer_ptr layer);
    void pop_overlay(layer_ptr overlay);

    void on_events();
    void on_update();
    void on_imgui_render();
    void on_render();

    template <typename unary_function> void for_each(unary_function &&f)
    {
        for (auto &&layer : layers_)
            f(layer);
    }

  private:
    std::vector<layer_ptr> layers_;
    std::size_t overlays_begin_ = 0;
};

} // namespace woxel
