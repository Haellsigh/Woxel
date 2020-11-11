#pragma once

#include "woxel_engine/core/log.hh"

extern woxel::application *woxel::create_application();

int main(int, char **)
{
    woxel::log::init();

    auto app = woxel::create_application();

    app->run();

    delete app;

    return 0;
}
