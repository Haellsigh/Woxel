# bgfx
set(BGFX_BUILD_EXAMPLES OFF)
add_subdirectory(bgfx.cmake)
add_library(bgfx::bgfx ALIAS bgfx)

# bgfx-imgui
add_subdirectory(bgfx-imgui)

# tracy
add_library(TracyClient STATIC "")
target_sources(TracyClient PRIVATE
  tracy/TracyClient.cpp
)
target_include_directories(TracyClient PUBLIC
  tracy
)
target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)
