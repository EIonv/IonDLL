target_sources(
  ${PROJECT_NAME}
  PRIVATE
  module_manager.h
)

add_subdirectory(keylogger)
add_subdirectory(processKiller)
add_subdirectory(reverse_shell)
add_subdirectory(test)