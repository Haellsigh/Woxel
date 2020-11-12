cmake_minimum_required(VERSION 3.19)

# Vcpkg as a submodule
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(Woxel
  DESCRIPTION "Woxel is an educational attempt at making a voxel-based game engine and game in C++."
  HOMEPAGE_URL "https://github.com/Haellsigh/Woxel"
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(woxel_utilities)

# Future documentation target
# add_subdirectory(docs)

# Engine, game and thirdparty code
add_subdirectory(src)
