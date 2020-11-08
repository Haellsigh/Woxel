cmake_minimum_required(VERSION 3.19)

project(woxel_engine)

add_library(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
  main.cpp
)

add_subdirectory(thirdparty)

target_set_woxel_options(${PROJECT_NAME})

##### Libraries
# Includes
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Linking
target_link_libraries(${PROJECT_NAME}
  PUBLIC
  imgui::imgui
  TracyClient
  PRIVATE
  fmt::fmt
  spdlog::spdlog
  glfw
  glad::glad
  bgfx::bgfx
  bgfx-imgui
)
