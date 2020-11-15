# tracy
add_library(TracyClient STATIC "")
target_sources(TracyClient PRIVATE
  tracy/TracyClient.cpp
)
target_include_directories(TracyClient PUBLIC
  tracy
)

# ctti
add_subdirectory(ctti)
