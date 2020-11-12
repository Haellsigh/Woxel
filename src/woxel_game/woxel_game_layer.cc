#include "woxel_game_layer.hh"

#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#include <woxel_engine/scene/system.hh>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#pragma warning(pop)
#include <imgui.h>

#include <fstream>
#include <iostream>

namespace fileops {

inline static std::streamoff stream_size(std::istream &file) {
    std::istream::pos_type current_pos = file.tellg();
    if (current_pos == std::istream::pos_type(-1)) { return -1; }
    file.seekg(0, std::istream::end);
    std::istream::pos_type end_pos = file.tellg();
    file.seekg(current_pos);
    return end_pos - current_pos;
}

inline bool stream_read_string(std::istream &file, std::string &fileContents) {
    std::streamoff len = stream_size(file);
    if (len == -1) { return false; }
    fileContents.resize(static_cast<std::string::size_type>(len));
    file.read(&fileContents[0], fileContents.length());
    return true;
}

inline bool read_file(const std::string &filename, std::string &fileContents) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        woxel::log::error("unable to open file {}", filename);
        return false;
    }
    const bool success = stream_read_string(file, fileContents);
    file.close();

    if (!success) { woxel::log::error("unable to read file {}", filename); }

    return success;
}

} // namespace fileops

struct PosColorVertex {
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cube_vertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},  //
    {1.0f, 1.0f, 1.0f, 0xff0000ff},   //
    {-1.0f, -1.0f, 1.0f, 0xff00ff00}, //
    {1.0f, -1.0f, 1.0f, 0xff00ffff},  //
};

static const uint16_t cube_tri_list[] = {0, 1, 2, 1, 3, 2};

static bgfx::ShaderHandle createShader(const std::string &shader, const char *name) {
    const bgfx::Memory *mem         = bgfx::copy(shader.data(), static_cast<uint32_t>(shader.size()));
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}

void renderer2d_system::on_attach() {
    auto &&reg = scene_->get_registry();
    (void)reg.group<transform, renderable_2d>();

    for (int i = 0; i < 100; i++) {
        auto entity = reg.create();
        reg.emplace<transform>(entity);
        reg.emplace<renderable_2d>(entity);
    }

    bgfx::VertexLayout pos_col_vert_layout;
    pos_col_vert_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    vbh_ = bgfx::createVertexBuffer(bgfx::makeRef(cube_vertices, sizeof(cube_vertices)), pos_col_vert_layout);
    ibh_ = bgfx::createIndexBuffer(bgfx::makeRef(cube_tri_list, sizeof(cube_tri_list)));

    std::string vshader;
    if (!fileops::read_file("assets/shaders/vs_cubes.bin", vshader)) { return; }

    std::string fshader;
    if (!fileops::read_file("assets/shaders/fs_cubes.bin", fshader)) { return; }

    vsh_ = createShader(vshader, "vshader");
    fsh_ = createShader(fshader, "fshader");

    program_ = bgfx::createProgram(vsh_, fsh_, true);
}

void renderer2d_system::on_detach() {}

void renderer2d_system::on_update() {
    ZoneScoped;
    scene_->get_registry().view<transform, renderable_2d>().each(
        [](auto, const auto &transform, const auto &renderable_2d) {
            ZoneScoped;
            // change color here depending on frame index
            (void)transform;
            (void)renderable_2d;
        });
}

void renderer2d_system::on_render() {
    ZoneScoped;

    scene_->get_registry().group<transform, renderable_2d>().each([&](auto, auto &transform, auto &renderable_2d) {
        ZoneScoped;
        (void)renderable_2d;

        auto view = glm::mat4(1.f);
        auto proj = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

        bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(proj));
        bgfx::setVertexBuffer(0, vbh_);
        bgfx::setIndexBuffer(ibh_);

        auto tr = glm::translate(transform.translation);
        bgfx::setTransform(glm::value_ptr(tr));

        bgfx::submit(0, program_);
    });
}

void woxel_game_layer::on_attach() {
    ZoneScoped;
    scene_.push_system(woxel::create_unique<renderer2d_system>());
}

void woxel_game_layer::on_detach() { ZoneScoped; }

void woxel_game_layer::on_events() {}

void woxel_game_layer::on_update() { scene_.on_update(); }

void woxel_game_layer::on_imgui_render() {
    ZoneScoped;
    ImGui::ShowDemoWindow();
}

void woxel_game_layer::on_render() { scene_.on_render(); }
