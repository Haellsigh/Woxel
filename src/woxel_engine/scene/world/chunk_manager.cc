#include "woxel_engine/scene/world/chunk_manager.hh"
#include "woxel_engine/debug/instrumentor.hh"
#include "woxel_engine/scene/world/chunk.hh"

namespace woxel {

void chunk_manager::on_attach() {}

void chunk_manager::on_detach() {}

void chunk_manager::on_update(const stopwatch &) {
    ZoneScoped;
    /*chunk_container_.for_around(chunk_position{0, 0}, 10, [](chunk_position const &position, chunk *chunk) {
        (void)position;
        (void)chunk;
    });*/
}

void chunk_manager::on_imgui_render() { ZoneScoped; }

void chunk_manager::on_render() {
    ZoneScoped;
    /*chunk_container_.for_around({0, 0}, 10, [](chunk_position const &position, chunk *chunk) {
        (void)position;
        (void)chunk;
    });*/
}

} // namespace woxel
