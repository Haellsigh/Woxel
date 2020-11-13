cmake_minimum_required(VERSION 3.17)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported LANGUAGES CXX)

##### Sets the compilation options
function(target_set_woxel_options target)
  # C++20
  target_compile_features(${target} PUBLIC cxx_std_20)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
  if (MSVC)
    target_compile_options(${target} PUBLIC /std:c++latest /Zc:__cplusplus)
  endif()
  # Enable Link Time Optimization if available
  if(ipo_supported)
    set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
  # Warnings
  if(MSVC)
    target_compile_options(${target} PRIVATE /WX /W4)
  else()
    target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
  endif()
endfunction()