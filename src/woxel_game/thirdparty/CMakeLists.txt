cmake_minimum_required(VERSION 3.19)

include(woxel_utilities)

# imnodes
add_library(imnodes STATIC)
target_sources(imnodes PRIVATE
    imnodes/imnodes.h
    imnodes/imnodes_internal.h
    imnodes/imnodes.cpp
)
# C++20
target_compile_features(imnodes PUBLIC cxx_std_20)
set_target_properties(imnodes PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
  target_compile_options(imnodes PUBLIC /std:c++latest /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
if(ipo_supported)
  set_target_properties(imnodes PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
# Warnings
if(MSVC)
  target_compile_options(imnodes PRIVATE /W3)
else()
  target_compile_options(imnodes PRIVATE -Wall -Wextra -Wall -Wconversion -Wsign-conversion)
endif()
find_package(imgui CONFIG REQUIRED)
target_link_libraries(imnodes PRIVATE
    imgui::imgui
)

# Includes
target_include_directories(woxel_game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
