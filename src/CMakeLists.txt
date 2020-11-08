add_executable(Woxel "")

target_sources(Woxel PRIVATE
  main.cpp
)

##### Compilation options
# C++20
target_compile_features(Woxel PUBLIC cxx_std_20)
set_target_properties(Woxel PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
  target_compile_options(Woxel PUBLIC /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(Woxel PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "Enabled Link Time optimization")
endif()
# Warnings
if(MSVC)
  target_compile_options(Woxel PRIVATE /WX /W4)
else()
  target_compile_options(Woxel PRIVATE -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
endif()
# CCache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "Enabled CCache")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

##### Pre build actions
# Copy resources to build folder
add_custom_command(TARGET Woxel PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:Woxel>/resources/)

##### Libraries
# Includes
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# Linking
target_link_libraries(Woxel PRIVATE
  fmt::fmt
  spdlog::spdlog
  glfw
  glad::glad
  bgfx::bgfx
  TracyClient
)
