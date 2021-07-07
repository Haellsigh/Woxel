cmake_minimum_required(VERSION 3.19)

# imgui-node-editor
set(IMGUI_NODE_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-node-editor)
add_library(imgui_node_editor STATIC)

target_sources(imgui_node_editor PRIVATE
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/crude_json.cpp
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/crude_json.h
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_bezier_math.h
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_bezier_math.inl
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_canvas.cpp
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_canvas.cpp
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_canvas.h
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_canvas.h
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_extra_math.h
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_extra_math.inl
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_node_editor_api.cpp
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_node_editor_internal.h
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_node_editor_internal.inl
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_node_editor.cpp
    ${IMGUI_NODE_EDITOR_ROOT_DIR}/imgui_node_editor.h
)

target_include_directories(imgui_node_editor PUBLIC ${IMGUI_NODE_EDITOR_ROOT_DIR})

find_package(imgui CONFIG REQUIRED)
target_link_libraries(imgui_node_editor PUBLIC
    imgui::imgui
)
