cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(Deliberation C CXX)

# Configuration options
option(OPTION_BUILD_STATIC "Build static libraries" OFF)

if(OPTION_BUILD_STATIC)
   set(BUILD_SHARED_LIBS OFF)
   message("Note: DELIBERATION_STATIC needs to be defined for static linking.")
else()
    set(BUILD_SHARED_LIBS ON)
endif()

# CMake configuration
set(CMAKE_MODULE_PATH 			   ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Setup platform specifics (compile flags, etc., ...)
if(MSVC)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
elseif(UNIX AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxClang.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
else()
    message(WARNING "Unsupported platform/compiler combination")
endif()

# Include projects
add_subdirectory(Source)
