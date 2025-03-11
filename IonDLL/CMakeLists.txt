cmake_minimum_required(VERSION 3.20)
project(ion LANGUAGES CXX C VERSION 1.0.1)

# Set C++ and C standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable LTO (Link Time Optimization) in Release builds
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default to static libraries, but allow override
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Set modern CMake policies
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()
if(POLICY CMP0092)
  cmake_policy(SET CMP0092 NEW)
endif()

# Include FetchContent module
include(FetchContent)

# Function to handle FetchContent with fallback to vcpkg
function(fetch_dependency NAME REPO TAG VCPKG_NAME)
  FetchContent_Declare(
    ${NAME}
    GIT_REPOSITORY ${REPO}
    GIT_TAG ${TAG}
  )
  FetchContent_GetProperties(${NAME})
  if(NOT ${NAME}_POPULATED)
    message(STATUS "Fetching ${NAME} from ${REPO}...")
    FetchContent_Populate(${NAME})
    if(NOT ${NAME}_POPULATED)
      message(WARNING "Failed to fetch ${NAME}. Falling back to vcpkg or system package.")
      find_package(${VCPKG_NAME} REQUIRED)
    else()
      add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
  endif()
endfunction()

# Detect Windows platform
if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601) # Windows 7+
endif()

# Fetch dependencies with fallback to vcpkg
fetch_dependency(spdlog https://github.com/gabime/spdlog.git v1.14.1 spdlog)
fetch_dependency(fmt https://github.com/fmtlib/fmt.git 10.2.1 fmt)
fetch_dependency(nlohmann_json https://github.com/nlohmann/json.git v3.11.3 nlohmann_json)

# Add the shared library
add_library(${PROJECT_NAME} SHARED IonDLL.cpp)

# Specify sources
target_sources(${PROJECT_NAME} PRIVATE
  IonDLL.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  spdlog::spdlog
  fmt::fmt
  nlohmann_json::nlohmann_json
  $<$<PLATFORM_ID:Windows>:crypt32>
  $<$<PLATFORM_ID:Windows>:ws2_32>
  $<$<PLATFORM_ID:Windows>:wininet>
  $<$<PLATFORM_ID:Windows>:iphlpapi>
)

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# Add subdirectories (assuming these exist)
add_subdirectory(debug)
add_subdirectory(modules)

# Installation rules
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# Export targets for downstream use
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# Generate package config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# Provide user guidance
if(NOT spdlog_POPULATED OR NOT fmt_POPULATED OR NOT nlohmann_json_POPULATED)
  message(STATUS "One or more dependencies could not be fetched via FetchContent.")
  message(STATUS "If you have vcpkg installed, you can install dependencies with:")
  message(STATUS "  vcpkg install spdlog fmt nlohmann-json")
  message(STATUS "Then run CMake with: -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake")
endif()