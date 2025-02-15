target_sources(
  iondll
  PRIVATE
  module_manager.cpp
)
add_subdirectory(resources)

add_subdirectory(test)

