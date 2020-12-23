#pragma once

#include "woxel_engine/scene/world/chunk.hh"
#include "woxel_engine/scene/world/types.hh"

#include <entt/signal/delegate.hpp>

#include <memory>
#include <unordered_map>

namespace woxel {

class chunk;

class chunk_container {
  public:
    chunk_container()  = default;
    ~chunk_container() = default;

    /*
    template <typename Callable> void for_around(world_position const &origin, int32_t radius, Callable &&func) {
        for_around(to_chunk_position(origin), radius, std::forward<Callable>(func));
    }

    template <typename Callable> void for_around(chunk_position const &origin, int32_t radius, Callable &&func) {
        for (int32_t dx = 0; std::abs(dx) <= radius; dx = -dx, dx -= (dx <= 0)) {
            for (int32_t dy = 0; std::abs(dy) <= radius; dy = -dy, dy -= (dy <= 0)) {
                const chunk_position position = origin + chunk_position{dx, dy};
                func(position, chunks_[position].get());
            }
        }
    }
    */

  private:
    // vector index to position

  private:
    // std::unordered_map<chunk_position, std::unique_ptr<chunk>> chunks_;
};

} // namespace woxel
