cmake_minimum_required(VERSION 3.19)

project(woxel_engine)

add_library(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
  woxel_engine.hh
)

add_subdirectory(core)
add_subdirectory(debug)
add_subdirectory(gui)

add_subdirectory(thirdparty)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_set_woxel_options(${PROJECT_NAME})


##### Libraries
# Includes
find_package(fmt CONFIG REQUIRED)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Taskflow CONFIG REQUIRED)

# Linking
target_link_libraries(${PROJECT_NAME}
  PUBLIC
  imgui::imgui
  TracyClient
  PRIVATE
  fmt::fmt
  spdlog::spdlog_header_only
  glfw
  glad::glad
  bgfx::bgfx
  bgfx-imgui
  Taskflow::Taskflow
)
