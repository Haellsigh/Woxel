#include "woxel_engine/debug/instrumentor.hh"

#include <Tracy.hpp>

#include <cstdlib>

void *operator new(std::size_t count)
{
    auto ptr = std::malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}
void operator delete(void *ptr) noexcept
{
    TracyFree(ptr);
    std::free(ptr);
}
