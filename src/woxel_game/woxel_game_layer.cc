#include "woxel_game_layer.hh"

#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#include <woxel_engine/scene/system.hh>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Platform/GLContext.h>
#include <imgui.h>

using namespace Magnum;

/* Setup the colored triangle */
using namespace Math::Literals;
struct TriangleVertex {
    Vector2 position;
};
const TriangleVertex data[]{
    {{-0.5f, -0.5f}}, //
    {{-0.5f, 0.5f}},  //
    {{0.5f, 0.5f}},   //
    {{-0.5f, -0.5f}}, //
    {{0.5f, 0.5f}},   //
    {{0.5f, -0.5f}}   //
};

void renderer2d_system::on_attach() {
    auto &&reg = scene_->get_registry();
    (void)reg.group<transform, renderable_2d>();

    for (int i = -5; i < 5; i++) {
        for (int j = -5; j < 5; j++) {
            auto entity = reg.create();
            reg.emplace<transform>(entity);
            reg.emplace<renderable_2d>(entity);

            reg.get<transform>(entity).translation = {1.1f * static_cast<float>(i), 1.1f * static_cast<float>(j), 0};
        }
    }

    /* Setup the colored triangle */
    buffer_ = woxel::create_unique<GL::Buffer>();
    buffer_->setData(data, GL::BufferUsage::StaticDraw);

    mesh_ = woxel::create_unique<GL::Mesh>();
    mesh_->setPrimitive(GL::MeshPrimitive::Triangles)
        .setCount(6)
        .addVertexBuffer(*buffer_, 0, Shaders::Flat2D::Position{});

    shader_ = woxel::create_unique<Shaders::Flat2D>();
}

void renderer2d_system::on_detach() {}

void renderer2d_system::on_update() {
    ZoneScoped;
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    scene_->get_registry().view<transform, renderable_2d>().each([&](auto, const auto &transform, auto &renderable_2d) {
        ZoneScoped;
        // change color here depending on frame index

        renderable_2d.color.r = static_cast<float>((time % 5000000000) / 500000000. + transform.translation.x / 5);
        renderable_2d.color.g = static_cast<float>((time % 1000000000) / 1000000000. + transform.translation.y / 5);
        renderable_2d.color.b = static_cast<float>((time % 10000000000) / 10000000000. +
                                                   transform.translation.x * transform.translation.y / 25);

        (void)transform;
    });
}

void renderer2d_system::on_render() {
    ZoneScoped;

    auto projection = Matrix3::projection({32.f, 18.f});

    scene_->get_registry().group<transform, renderable_2d>().each([&](auto, auto &transform, auto &renderable_2d) {
        ZoneScoped;

        auto translation = Matrix3::translation({transform.translation.x, transform.translation.y});
        auto matrix      = projection * translation;
        shader_->setTransformationProjectionMatrix(matrix);
        shader_->setColor({renderable_2d.color.r, renderable_2d.color.g, renderable_2d.color.b, renderable_2d.color.a});
        shader_->draw(*mesh_);
    });
}

void woxel_game_layer::on_attach() {
    ZoneScoped;
    scene_.push_system(woxel::create_unique<renderer2d_system>());
}

void woxel_game_layer::on_detach() { ZoneScoped; }

void woxel_game_layer::on_update() { scene_.on_update(); }

void woxel_game_layer::on_imgui_render() {
    ZoneScoped;
    ImGui::ShowDemoWindow();
}

void woxel_game_layer::on_render() { scene_.on_render(); }
