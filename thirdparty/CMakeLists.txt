# bgfx
set(BGFX_BUILD_EXAMPLES OFF)
add_subdirectory(bgfx.cmake)
add_library(bgfx::bgfx ALIAS bgfx)
