cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(Deliberation C CXX)

set(DELIBERATION_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Build Type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DELIBERATION_BUILD_TYPE_DEBUG 1)
    set(DELIBERATION_BUILD_TYPE_RELEASE 0)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(DELIBERATION_BUILD_TYPE_DEBUG 0)
    set(DELIBERATION_BUILD_TYPE_RELEASE 1)
else()
    message(FATAL_ERROR "Unknown build type ${CMAKE_BUILD_TYPE}")
endif()

# Configuration options

# CMake configuration
set(CMAKE_MODULE_PATH 			   ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD             14)

# Setup platform specifics (compile flags, etc., ...)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Platforms/Platform.cmake)

# dependencies
find_package(OpenGL         REQUIRED)
find_package(glbinding      REQUIRED)
find_package(cxxopts        REQUIRED)
find_package(GLM            REQUIRED)
find_package(SDL2           REQUIRED)
find_package(SDL2_ttf       REQUIRED)
find_package(SDL2_image     REQUIRED)
find_package(json           REQUIRED)
find_package(imgui          REQUIRED)
find_package(boost          REQUIRED)
find_package(tinyobjloader  REQUIRED)

# Include projects
add_subdirectory(Extern/googletest-1.8.0)
add_subdirectory(Source)
