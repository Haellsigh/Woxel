#pragma once

#include <woxel_engine/core/layer.hh>
#include <woxel_engine/scene/scene.hh>
#include <woxel_engine/scene/system.hh>
#include <woxel_engine/woxel_engine.hh>

#include <Magnum/GL/Buffer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector4.h>
#include <Magnum/Shaders/Phong.h>
#include <glm/glm.hpp>

namespace Magnum::GL {
class Mesh;
} // namespace Magnum::GL

constexpr uint32_t max_vertices_ = 32768 << 4;
constexpr uint32_t kMaxDims      = 32;
constexpr float kMaxDimsF        = float(kMaxDims);

struct Vertex {
    Magnum::Vector3 position;
    Magnum::Vector3 normal;
    Magnum::Color3 color;
};

struct transform {
    glm::vec3 translation = {0.f, 0.f, 0.f};
    glm::vec3 rotation    = {0.f, 0.f, 0.f};
    glm::vec3 scale       = {1.f, 1.f, 1.f};
};

struct renderable_2d {
    glm::vec4 color = {1.f, 1.f, 1.f, 1.f};
};

struct Grid {
    float m_val              = 0.f;
    Magnum::Vector3 m_normal = {0.f, 0.f, 0.f};
};

class renderer2d_system : public woxel::system {
    void on_attach() final;
    void on_detach() final;

    void on_update() final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    std::chrono::high_resolution_clock::time_point start_time_;

    Magnum::Vector4 light_pos_ = {0.f, 0.f, -1.f, 0.f};
    float iso_                 = 0.75f;
    float shininess_           = 10.f;
    uint32_t num_dims_         = kMaxDims;

    woxel::unique<Magnum::GL::Mesh> mesh_;
    woxel::unique<Magnum::Shaders::Phong> shader_;
    std::array<Vertex, max_vertices_> data_;
    woxel::unique<Magnum::GL::Buffer> buffer_;
    uint32_t num_vertices_;

    std::array<Grid, kMaxDims * kMaxDims * kMaxDims> grid_;
};

class woxel_game_layer : public woxel::layer {
  public:
    woxel_game_layer()  = default;
    ~woxel_game_layer() = default;

    void on_attach() final;
    void on_detach() final;
    void on_update() final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    woxel::scene scene_;
};
