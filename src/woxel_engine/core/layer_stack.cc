#include "woxel_engine/core/layer_stack.hh"
#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"

namespace woxel {

layer_stack::~layer_stack() {
    ZoneScoped;
    for (auto &&layer : layers_) {
        layer->on_detach();
    }
}

void layer_stack::push_layer(layer_ptr layer) {
    ZoneScoped;
    const auto &pos = std::next(layers_.begin(), layers_count_);
    layers_.emplace(pos, std::move(layer));

    layers_[layers_count_++]->on_attach();
}

void layer_stack::push_overlay(layer_ptr overlay) {
    ZoneScoped;
    layers_.push_back(std::move(overlay));
    layers_.back()->on_attach();
}

void layer_stack::pop_layer(layer_ptr layer) {
    ZoneScoped;
    const auto &end = std::next(layers_.begin(), layers_count_);
    auto it = std::find(layers_.begin(), end, layer);
    if (it != end) {
        layer->on_detach();
        layers_.erase(it);
        --layers_count_;
    }
}

void layer_stack::pop_overlay(layer_ptr overlay) {
    ZoneScoped;
    const auto &start = std::next(layers_.begin(), layers_count_);
    auto it = std::find(start, layers_.end(), overlay);
    if (it != layers_.end()) {
        overlay->on_detach();
        layers_.erase(it);
    }
}

} // namespace woxel
