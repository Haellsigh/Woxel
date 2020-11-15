#include "woxel_engine/debug/instrumentor.hh"

#include <cstdlib>

auto operator new(std::size_t count) -> void *
{
    auto *ptr = std::malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}
void operator delete(void *ptr) noexcept {
    TracyFree(ptr);
    std::free(ptr);
}
