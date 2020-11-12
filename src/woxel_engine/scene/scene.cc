#include "woxel_engine/scene/scene.hh"
#include "woxel_engine/core/log.hh"
#include "woxel_engine/scene/system.hh"

namespace woxel {

scene::~scene() {
    for (auto &&system : systems_) {
        system->on_detach();
    }
}

void scene::push_system(unique<system> system) {
    system->set_scene(this);
    systems_.emplace_back(std::move(system))->on_attach();
}

auto scene::get_registry() -> entt::registry & { return registry_; }

void scene::on_update() {
    for (auto &&system : systems_) {
        system->on_update();
    }
}

void scene::on_render() {
    for (auto &&system : systems_) {
        system->on_render();
    }
}

} // namespace woxel
