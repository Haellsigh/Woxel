cmake_minimum_required(VERSION 3.19)

project(woxel_engine)

add_library(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
    woxel_engine.hh
)

add_subdirectory(core)
add_subdirectory(debug)
add_subdirectory(gui)
add_subdirectory(scene)
add_subdirectory(thirdparty)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_set_woxel_options(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PUBLIC /wd4251) # disable warning C4251
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<Magnum/Platform/GLContext.h>)
# Enable tracy in debug mode only
#target_compile_definitions(TracyClient PUBLIC "$<$<CONFIG:DEBUG>:TRACY_ENABLE>")
target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE)

##### Libraries
# Includes
find_package(fmt CONFIG REQUIRED)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Taskflow CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Corrade CONFIG REQUIRED Main)
find_package(Magnum CONFIG REQUIRED COMPONENTS
    GL
    WglContext
    Shaders
    MeshTools
    Primitives
)
#find_package(MagnumIntegration REQUIRED Glm)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Linking
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    imgui::imgui
    TracyClient
    EnTT::EnTT
    glm
    #PRIVATE
    fmt::fmt
    spdlog::spdlog_header_only
    glfw
    Corrade::Main
    Magnum::Magnum
    Magnum::GL
    Magnum::GLContext
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    #MagnumIntegration::Glm
    Taskflow::Taskflow
)
