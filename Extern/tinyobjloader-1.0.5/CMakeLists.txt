add_library(tinyobjloader
    tinyobjloader/tiny_obj_loader.cpp
    tinyobjloader/tiny_obj_loader.h
)

target_include_directories(tinyobjloader
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader)