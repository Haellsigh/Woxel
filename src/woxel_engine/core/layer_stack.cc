#include "woxel_engine/core/layer_stack.hh"
#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"

namespace woxel
{

layer_stack::~layer_stack()
{
    ZoneScoped;
    for (const auto &layer : layers_)
    {
        layer->on_detach();
    }
}

void layer_stack::push_layer(std::unique_ptr<layer> layer)
{
    ZoneScoped;
    const auto &pos = std::next(layers_.begin(), overlays_begin_);
    layers_.emplace(pos, std::move(layer));

    woxel::log::trace("calling on_attach");
    layers_[overlays_begin_++]->on_attach();
}

void layer_stack::push_overlay(std::unique_ptr<layer> overlay)
{
    ZoneScoped;
    layers_.push_back(std::move(overlay));
    layers_.back()->on_attach();
}

void layer_stack::pop_layer(std::unique_ptr<layer> layer)
{
    ZoneScoped;
    const auto &end = std::next(layers_.begin(), overlays_begin_);
    auto it = std::find(layers_.begin(), end, layer);
    if (it != end)
    {
        layer->on_detach();
        layers_.erase(it);
        --overlays_begin_;
    }
}

void layer_stack::pop_overlay(std::unique_ptr<layer> overlay)
{
    ZoneScoped;
    const auto &start = std::next(layers_.begin(), overlays_begin_);
    auto it = std::find(start, layers_.end(), overlay);
    if (it != layers_.end())
    {
        overlay->on_detach();
        layers_.erase(it);
    }
}

void layer_stack::on_events()
{
}

void layer_stack::on_update()
{
}

void layer_stack::on_imgui_render()
{
}

void layer_stack::on_render()
{
}

} // namespace woxel
