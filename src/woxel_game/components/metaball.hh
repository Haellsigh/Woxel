#pragma once

#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Vector3.h>

#include <cstdint>
#include <vector>

struct metaball {
    using vec3 = Magnum::Math::Vector3<float>;
    using mat4 = Magnum::Math::Matrix4<float>;

    // constants
    static constexpr uint32_t k_max_dims     = 32;
    static constexpr float k_max_dims_float  = float(k_max_dims);
    static constexpr uint32_t k_max_vertices = 1 << 15; // 32768;
    static constexpr uint32_t k_num_spheres  = 16;

    // types
    struct grid {
        float val_   = {0.f};
        vec3 normal_ = {0.f, 0.f, 0.f};
    };

    struct vertex {
        vec3 position_;
        vec3 normal_;
        vec3 color_;
    };

    // variables
    uint32_t max_dims_ = k_max_dims, num_dims_ = k_max_dims;
    uint32_t num_vertices_ = 0;
    float iso_             = 0.75f;
    float rot_x_ = 0.67f, rot_y_ = 1.f;
    float time_multiplier_ = 1.f;

    std::vector<grid> grid_{k_max_dims * k_max_dims * k_max_dims};
    std::vector<vertex> vertices_{k_max_vertices};
    vec3 position_;
    mat4 transform_;
};
