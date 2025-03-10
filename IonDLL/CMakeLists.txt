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

set(BUILD_SHARED_LIBS OFF)

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

include(FetchContent)

# Fetch spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2  # You can specify the version you need
)

FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.0.1  # You can specify the version you need
)
  
FetchContent_MakeAvailable(fmt)
  
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.9.1  # You can specify the version you need
)

FetchContent_MakeAvailable(nlohmann_json)

# Add the library
add_library(${PROJECT_NAME} SHARED IonDLL.cpp)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  IonDLL.cpp
)

# Link spdlog to your project
target_link_libraries(${PROJECT_NAME} PRIVATE 
  spdlog::spdlog 
  fmt::fmt 
  nlohmann_json::nlohmann_json
  crypt32
)

# we are going to add the include directory for the project
add_subdirectory(debug)
add_subdirectory(modules)