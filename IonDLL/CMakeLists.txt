cmake_minimum_required(VERSION 3.20)
project(ion LANGUAGES CXX C VERSION 1.0.1)

# Set C++ and C standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable LTO (Link Time Optimization)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

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

# Function to handle FetchContent failures with a helpful message
function(fetch_dependency NAME REPO TAG)
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
      message(WARNING "Failed to fetch ${NAME}. Ensure you have git installed and internet access.")
      message(WARNING "Alternatively, consider using vcpkg: 'vcpkg install ${NAME}' and adjust CMake to use find_package.")
    endif()
    add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endfunction()

# Fetch dependencies
fetch_dependency(spdlog https://github.com/gabime/spdlog.git v1.9.2)
fetch_dependency(fmt https://github.com/fmtlib/fmt.git 8.0.1)
fetch_dependency(nlohmann_json https://github.com/nlohmann/json.git v3.9.1)

# Add the shared library
add_library(${PROJECT_NAME} SHARED IonDLL.cpp)

# Specify sources (simplified from target_sources)
target_sources(${PROJECT_NAME} PRIVATE IonDLL.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  spdlog::spdlog
  fmt::fmt
  nlohmann_json::nlohmann_json
  crypt32
  ws2_32
  wininet
)

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# Add subdirectories
add_subdirectory(debug)
add_subdirectory(modules)

# Installation rules (optional)
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# Provide user guidance if vcpkg is preferred
if(NOT spdlog_POPULATED OR NOT fmt_POPULATED OR NOT nlohmann_json_POPULATED)
  message(STATUS "One or more dependencies could not be fetched via FetchContent.")
  message(STATUS "If you have vcpkg installed, you can install dependencies with:")
  message(STATUS "  vcpkg install spdlog fmt nlohmann-json")
  message(STATUS "Then modify CMakeLists.txt to use find_package instead of FetchContent.")
endif()