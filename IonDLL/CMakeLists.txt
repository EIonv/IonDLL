cmake_minimum_required(VERSION 3.20)
project(iondll LANGUAGES CXX C VERSION 1.0.1)

include(FetchContent)

# Fetch spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2  # You can specify the version you need
)

FetchContent_MakeAvailable(spdlog)

add_library(${PROJECT_NAME} SHARED IonDLL.cpp)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  IonDLL.cpp
)

# Link spdlog to your project
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

# we are going to add the include directory for the project
add_subdirectory(modules)
add_subdirectory(resources)
