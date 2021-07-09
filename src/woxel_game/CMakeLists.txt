cmake_minimum_required(VERSION 3.19)

# Project
project(woxel_game)

# Executable
add_executable(${PROJECT_NAME} "")

# Sources
target_sources(${PROJECT_NAME} PRIVATE
  woxel_app.cpp
  woxel_game_layer.cc
  woxel_game_layer.hh
)

add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(thirdparty)

# Compilation options
target_set_woxel_options(${PROJECT_NAME})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui-node-editor/misc/cmake-modules)

# Pre build actions
# Copy assets to build folder
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${woxel_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:woxel_game>/assets/
)

message(STATUS "Module paths: ${CMAKE_MODULE_PATH}")

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE woxel_engine imnodes)
