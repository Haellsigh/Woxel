#include "woxel_engine/scene/world/types.hh"

namespace woxel {

chunk_position to_chunk_position(world_position const &position) {
    const glm::vec<2, int32_t, glm::defaultp> position_integer = position;
    const int32_t x =
        (position_integer.x / chunk_size_x) + ((position_integer.x % chunk_size_x) >> ((sizeof(int32_t) << 3) - 1));
    const int32_t y =
        (position_integer.y / chunk_size_y) + ((position_integer.y % chunk_size_y) >> ((sizeof(int32_t) << 3) - 1));

    return {x, y};
}

} // namespace woxel
