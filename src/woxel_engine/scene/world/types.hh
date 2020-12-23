#pragma once

#include <glm/glm.hpp>

namespace woxel {

constexpr std::int32_t chunk_size_x = 16;
constexpr std::int32_t chunk_size_y = 16;
using chunk_position                = glm::vec<2, int32_t, glm::defaultp>;

using world_position = glm::dvec3;

chunk_position to_chunk_position(world_position const &position);

} // namespace woxel
