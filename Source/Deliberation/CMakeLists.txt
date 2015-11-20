message(STATUS "Deliberation")

# defines
set(include_path 	"${CMAKE_CURRENT_SOURCE_DIR}/include/Deliberation")
set(source_path 	"${CMAKE_CURRENT_SOURCE_DIR}/source")

# packages
find_package(glbinding  REQUIRED)
find_package(globjects  REQUIRED)
find_package(GLM        REQUIRED)

# include
include_directories(
    BEFORE

    ${CMAKE_CURRENT_SOURCE_DIR}/include

    ${GLBINDING_INCLUDE_DIR}
    ${GLOBJECTS_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# sources
set(sources
    ${source_path}/Core/Assert.cpp
    ${source_path}/Core/Blob.cpp

    ${source_path}/Draw/Detail/BufferImpl.cpp
    ${source_path}/Draw/Detail/BufferImpl.h
    ${source_path}/Draw/GL/GLSLSizeOf.cpp
    ${source_path}/Draw/GL/GLStateManager.cpp
    ${source_path}/Draw/Buffer.cpp
    ${source_path}/Draw/BufferLayout.cpp
    ${source_path}/Draw/BufferLayoutField.cpp
    ${source_path}/Draw/BufferUpload.cpp
    ${source_path}/Draw/BufferUploadExecution.cpp
    ${source_path}/Draw/BufferUploadExecution.h
    ${source_path}/Draw/Context.cpp
)

# api_includes
set(api_includes
    ${include_path}/Deliberation_API.h

    ${include_path}/Core/Detail/IBlobImpl.h
    ${include_path}/Core/Assert.h
    ${include_path}/Core/Blob.h
    ${include_path}/Core/Blob.inl
    ${include_path}/Core/OffsetOf.h
    ${include_path}/Core/OffsetOf.inl

    ${include_path}/Draw/Detail/INamedDataMemberOfImpl.h
    ${include_path}/Draw/Detail/INamedDataMemberOfImpl.inl
    ${include_path}/Draw/Detail/NamedDataMemberOf.h
    ${include_path}/Draw/Detail/NamedDataMemberOf.inl
    ${include_path}/Draw/GL/GLSLSizeOf.h
    ${include_path}/Draw/GL/GLStateManager.h
    ${include_path}/Draw/GL/GLSLTypeOf.h
    ${include_path}/Draw/GL/GLSLTypeOf.inl
    ${include_path}/Draw/Buffer.h
    ${include_path}/Draw/BufferLayout.h
    ${include_path}/Draw/BufferLayoutField.h
    ${include_path}/Draw/BufferUpload.h
    ${include_path}/Draw/Context.h
    ${include_path}/Draw/Context.inl
)

# libs
set(libs
    ${GLBINDING_LIBRARY}
    ${GLOBJECTS_LIBRARY}
)

# library
add_library(Deliberation ${api_includes} ${sources})
