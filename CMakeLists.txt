cmake_minimum_required(VERSION 3.19)

# Vcpkg as a submodule
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(Woxel
  VERSION 0.1.0
  DESCRIPTION "Woxel is an educational attempt at making a voxel-based game engine and game in C++."
  HOMEPAGE_URL "https://github.com/Haellsigh/Woxel"
  LANGUAGES CXX)

# Future documentation target
# add_subdirectory(docs)

#
add_subdirectory(thirdparty)
add_subdirectory(src)
