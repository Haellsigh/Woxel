#pragma once

#include <memory>

namespace woxel
{

class layer
{
  public:
    layer() = default;
    virtual ~layer() = default;

    virtual void on_attach()
    {
    }
    virtual void on_detach()
    {
    }

    virtual void on_events()
    {
    }
    virtual void on_update()
    {
    }
    virtual void on_imgui_render()
    {
    }
    virtual void on_render()
    {
    }
};

template <typename T, typename... Args> auto create_layer(Args... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

} // namespace woxel
