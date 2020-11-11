#include "woxel_game/woxel_game_layer.hh"

#include <woxel_engine/woxel_engine.hh>
//
#include <woxel_engine/core/entry_point.hh>
#include <woxel_engine/debug/instrumentor.hh>

class woxel_app final : public woxel::application
{
  public:
    woxel_app()
    {
        ZoneScoped;
        woxel::log::info("woxel_app::woxel_app");
        push_layer(woxel::create_layer<woxel_game_layer>());
    }

    ~woxel_app() final
    {
        woxel::log::info("woxel_app::~woxel_app");
    };
};

auto woxel::create_application() -> woxel::application *
{
    return new woxel_app();
}
