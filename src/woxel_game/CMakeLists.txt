cmake_minimum_required(VERSION 3.19)

project(woxel_game)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
  main.cpp
)

target_set_woxel_options(${PROJECT_NAME})

##### Pre build actions
# Copy assets to build folder
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${Woxel_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:woxel_game>/assets/)

##### Libraries
# Includes

# Linking
target_link_libraries(${PROJECT_NAME} PRIVATE woxel_engine)
