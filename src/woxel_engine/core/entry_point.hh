#pragma once

#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"

extern woxel::application *woxel::create_application();

auto operator new(std::size_t count) -> void * {
    auto *ptr = std::malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}

void operator delete(void *ptr) noexcept {
    TracyFree(ptr);
    std::free(ptr);
}

int main(int, char **) {
    woxel::log::init();

    auto app = woxel::create_application();

    app->run();

    delete app;

    return 0;
}
