cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(Deliberation C CXX)

set(DELIBERATION_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Configuration options

# CMake configuration
set(CMAKE_MODULE_PATH 			   ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD             14)

# Setup platform specifics (compile flags, etc., ...)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Platforms/Platform.cmake)

# dependencies
find_package(Bandit     REQUIRED)
find_package(OpenGL     REQUIRED)
find_package(glbinding  REQUIRED)
find_package(cxxopts    REQUIRED)
find_package(GLM        REQUIRED)
find_package(SDL2       REQUIRED)
find_package(SDL2_ttf   REQUIRED)
find_package(SDL2_image REQUIRED)

# Include projects
add_subdirectory(Extern/googletest-1.8.0)
add_subdirectory(Source)
