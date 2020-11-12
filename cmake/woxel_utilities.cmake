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

function(target_add_shader target shader_path shader_type)
  get_filename_component(shader_dir ${shader_path} DIRECTORY)
  get_filename_component(shader_ext ${shader_path} LAST_EXT)
  get_filename_component(shader_name ${shader_path} NAME_WLE)

  add_custom_command(
    OUTPUT ${${target}_BINARY_DIR}/assets/shaders/${shader_dir}${shader_name}.bin
    COMMAND shaderc -f ${shader_path} -o ${${target}_BINARY_DIR}/assets/shaders/${shader_dir}${shader_name}.bin --type ${shader_type} -i ./ -p spirv
    DEPENDS ${woxel_SOURCE_DIR}/assets/shaders/${shader_path}
    WORKING_DIRECTORY ${woxel_SOURCE_DIR}/assets/shaders
    COMMENT "Compiling shader assets/shaders/${shader_path}"
    VERBATIM
  )
  add_custom_target(
    compile_shader_${shader_path} ALL DEPENDS ${${target}_BINARY_DIR}/assets/shaders/${shader_dir}${shader_name}.bin
  )
  target_sources(${target} PRIVATE ${woxel_SOURCE_DIR}/assets/shaders/${shader_path})
  message(STATUS "Adding shader assets/shaders/${shader_path}")
endfunction()
