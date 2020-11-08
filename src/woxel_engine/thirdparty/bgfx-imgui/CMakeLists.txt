cmake_minimum_required(VERSION 3.19)

project(bgfx-imgui LANGUAGES CXX)

add_library(bgfx-imgui STATIC "")
target_sources(bgfx-imgui PRIVATE
  imgui_impl_bgfx.h
  imgui_impl_bgfx.cpp
  vs_ocornut_imgui.bin.h
  fs_ocornut_imgui.bin.h
)
target_include_directories(bgfx-imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(imgui CONFIG REQUIRED)
target_link_libraries(bgfx-imgui PRIVATE bgfx::bgfx imgui::imgui)
