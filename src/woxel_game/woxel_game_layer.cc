#include "woxel_game_layer.hh"

#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#include <woxel_engine/scene/system.hh>

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
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},   //
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},  //
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},  //
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff}, //
};

static const uint16_t cube_tri_list[] = {
    0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6, //
    1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7, //
};

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

void renderer2d_system::on_detach() {
    bgfx::destroy(vbh_);
    bgfx::destroy(ibh_);
    bgfx::destroy(program_);
}

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

        float view[16];
        bx::mtxIdentity(view);
        float proj[16];
        bx::mtxOrtho(proj, -2.f, 2.f, -2.f, 2.f, -1.f, 1.f, 0.f, bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(0, view, proj);
        bgfx::setVertexBuffer(0, vbh_);
        bgfx::setIndexBuffer(ibh_);

        float model[16];
        bx::mtxIdentity(model);
        bx::mtxTranslate(model, transform.translation.x, transform.translation.y, transform.translation.z);
        bgfx::setTransform(model);

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
