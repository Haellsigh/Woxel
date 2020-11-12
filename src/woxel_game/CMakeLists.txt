cmake_minimum_required(VERSION 3.19)

project(woxel_game)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
  woxel_app.cpp
  woxel_game_layer.cc
  woxel_game_layer.hh
)

target_set_woxel_options(${PROJECT_NAME})

##### Pre build actions
# Copy assets to build folder
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${Woxel_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:woxel_game>/assets/
)

# Compile shaders
#add_custom_command(
#  TARGET ${PROJECT_NAME} POST_BUILD
#  COMMAND shaderc -f vs_cubes.sc -o vs_cubes.bin --type vertex   -i ./ -p spirv
#  COMMAND shaderc -f fs_cubes.sc -o fs_cubes.bin --type fragment -i ./ -p spirv
#  COMMAND ${CMAKE_COMMAND} -E echo "Compiled shaders in assets/shaders"
#  DEPENDS ${woxel_game_BINARY_DIR}/assets/shaders/vs_cubes.sc ${woxel_game_BINARY_DIR}/assets/shaders/fs_cubes.sc ${woxel_game_BINARY_DIR}/assets/shaders/bgfx_shader.sh
#  COMMENT "Compiling shaders in assets/shaders"
#  WORKING_DIRECTORY ${woxel_game_BINARY_DIR}/assets/shaders
#  VERBATIM
#)
add_custom_command(
  OUTPUT  ${woxel_game_BINARY_DIR}/assets/shaders/vs_cubes.bin ${woxel_game_BINARY_DIR}/assets/shaders/fs_cubes.bin
  COMMAND shaderc -f vs_cubes.sc -o vs_cubes.bin --type vertex   -i ./ -p spirv
  COMMAND shaderc -f fs_cubes.sc -o fs_cubes.bin --type fragment -i ./ -p spirv
  COMMAND ${CMAKE_COMMAND} -E echo "Compiled shaders in assets/shaders"
  DEPENDS ${woxel_game_BINARY_DIR}/assets/shaders/vs_cubes.sc ${woxel_game_BINARY_DIR}/assets/shaders/fs_cubes.sc ${woxel_game_BINARY_DIR}/assets/shaders/bgfx_shader.sh
  COMMENT "Compiling shaders in assets/shaders"
  WORKING_DIRECTORY ${woxel_game_BINARY_DIR}/assets/shaders
  VERBATIM
)
add_custom_target(
  compile_shaders ALL
  DEPENDS ${woxel_game_BINARY_DIR}/assets/shaders/vs_cubes.bin ${woxel_game_BINARY_DIR}/assets/shaders/fs_cubes.bin
)

##### Libraries
# Includes

# Linking
target_link_libraries(${PROJECT_NAME} PRIVATE woxel_engine)
