set(TINYOBJLOADER_DIR ${PROJECT_SOURCE_DIR}/Extern/tinyobjloader-1.0.5)

include_directories(${TINYOBJLOADER_DIR})

add_library(tinyobjloader
    STATIC
    ${TINYOBJLOADER_DIR}/tiny_obj_loader.cpp
    ${TINYOBJLOADER_DIR}/tiny_obj_loader.h
    )

set(TINYOBJLOADER_LIBRARIES tinyobjloader)
set(TINYOBJLOADER_INCLUDE_DIRS ${TINYOBJLOADER_DIR})
 
