#pragma once

#include "woxel_engine/core/time.hh"

#include <Magnum/GL/AbstractShaderProgram.h>

namespace woxel {

class chunk {
  public:
    chunk()  = default;
    ~chunk() = default;

    void on_update(const stopwatch &s);
    void on_render(Magnum::GL::AbstractShaderProgram &shader);
};

} // namespace woxel
