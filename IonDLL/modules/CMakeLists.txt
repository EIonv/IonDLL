target_sources(
  ${PROJECT_NAME}
  PRIVATE
  module_manager.h
)

add_subdirectory(detach)
add_subdirectory(keylogger)
add_subdirectory(test)