#pragma once

#include "woxel_engine/core/base.hh"
#include "woxel_engine/core/layer.hh"
#include "woxel_engine/debug/instrumentor.hh"

#include <memory>
#include <vector>

namespace woxel {

class layer_stack {
  public:
    using layer_ptr = unique<layer>;

    layer_stack() = default;
    ~layer_stack();

    void push_layer(layer_ptr layer);
    void push_overlay(layer_ptr overlay);

    void pop_layer(layer_ptr layer);
    void pop_overlay(layer_ptr overlay);

    template <typename message_type> bool publish(const message_type &message) {
        ZoneScoped;

        // from last overlay to first layer in reverse
        auto end = std::prev(layers_.rend(), layers_count_);
        if (layers_count_ > 0) { std::advance(end, 1); }

        for (auto layer_it = layers_.rbegin(); layer_it != end; ++layer_it)
            if ((*layer_it)->publish(message)) return true;

        return false;
    }

    template <typename unary_function> void each(unary_function &&f) {
        ZoneScoped;

        // from first layer to last overlay
        auto start = std::next(layers_.begin(), layers_count_);
        if (layers_count_ > 0) { std::advance(start, -1); }

        for (auto layer_it = start; layer_it != layers_.end(); ++layer_it)
            f(*layer_it);
    }

  private:
    std::vector<layer_ptr> layers_;
    std::size_t layers_count_ = 0;
};

} // namespace woxel
