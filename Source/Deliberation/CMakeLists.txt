message(STATUS "Deliberation")

# defines
set(include_path 	"${CMAKE_CURRENT_SOURCE_DIR}/include/Deliberation")
set(source_path 	"${CMAKE_CURRENT_SOURCE_DIR}/source")

# packages
find_package(glbinding  REQUIRED)
find_package(GLM        REQUIRED)
FIND_PACKAGE(IL		    REQUIRED)
FIND_PACKAGE(ILU	    REQUIRED)
FIND_PACKAGE(SDL2	    REQUIRED)

# include
include_directories(
    BEFORE

    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source

    ${GLBINDING_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${IL_INCLUDE_DIR}
)

# library type
if (OPTION_BUILD_STATIC)
    add_definitions("-DDELIBERATION_STATIC")
else()
    add_definitions("-DDELIBERATION_EXPORTS")
endif()

# sources
set(sources
    ${source_path}/Deliberation.cpp

    ${source_path}/Core/Assert.cpp
    ${source_path}/Core/Blob.cpp
    ${source_path}/Core/FileUtils.cpp
    ${source_path}/Core/StringUtils.cpp
    ${source_path}/Core/Viewport.cpp

    ${source_path}/Draw/Detail/ClearImpl.cpp
    ${source_path}/Draw/Detail/ClearImpl.h
    ${source_path}/Draw/Detail/BufferImpl.cpp
    ${source_path}/Draw/Detail/BufferImpl.h
    ${source_path}/Draw/Detail/DrawExecution.cpp
    ${source_path}/Draw/Detail/DrawExecution.h
    ${source_path}/Draw/Detail/DrawImpl.cpp
    ${source_path}/Draw/Detail/DrawImpl.h
    ${source_path}/Draw/Detail/ILUtils.cpp
    ${source_path}/Draw/Detail/ILUtils.h
    ${source_path}/Draw/Detail/ITextureLoaderImpl.cpp
    ${source_path}/Draw/Detail/ITextureLoaderImpl.h
    ${source_path}/Draw/Detail/ProgramImpl.cpp
    ${source_path}/Draw/Detail/ProgramImpl.h
    ${source_path}/Draw/Detail/TextureBindingImpl.cpp
    ${source_path}/Draw/Detail/TextureBindingImpl.h
    ${source_path}/Draw/Detail/TextureLoaderCubeFilesImpl.cpp
    ${source_path}/Draw/Detail/TextureLoaderCubeFilesImpl.h
    ${source_path}/Draw/Detail/TextureLoaderDataImpl.cpp
    ${source_path}/Draw/Detail/TextureLoaderDataImpl.h
    ${source_path}/Draw/Detail/TextureLoaderFileImpl.cpp
    ${source_path}/Draw/Detail/TextureLoaderFileImpl.h
    ${source_path}/Draw/Detail/TextureLoaderSDLSurfaceImpl.cpp
    ${source_path}/Draw/Detail/TextureLoaderSDLSurfaceImpl.h
    ${source_path}/Draw/Detail/UniformImpl.cpp
    
    ${source_path}/Draw/GL/GLSLSizeOf.cpp
    ${source_path}/Draw/GL/GLStateManager.cpp
    ${source_path}/Draw/GL/GLUtils.cpp
    ${source_path}/Draw/GL/GLUtils.h
    ${source_path}/Draw/GL/GLVertexAttributeBinder.cpp
    ${source_path}/Draw/GL/GLVertexAttributeBinder.h
    ${source_path}/Draw/GL/GLVertexAttributeFormat.cpp
    ${source_path}/Draw/GL/GLVertexAttributeFormat.h
    
    ${source_path}/Draw/BlendState.cpp
    ${source_path}/Draw/Buffer.cpp
    ${source_path}/Draw/BufferLayout.cpp
    ${source_path}/Draw/BufferLayoutField.cpp
    ${source_path}/Draw/BufferUpload.cpp
    ${source_path}/Draw/BufferUploadExecution.cpp
    ${source_path}/Draw/BufferUploadExecution.h
    ${source_path}/Draw/Clear.cpp
    ${source_path}/Draw/ClearExecution.cpp
    ${source_path}/Draw/ClearExecution.h
    ${source_path}/Draw/Context.cpp
    ${source_path}/Draw/CullState.cpp
    ${source_path}/Draw/DepthState.cpp
    ${source_path}/Draw/Draw.cpp
    ${source_path}/Draw/DrawVerification.cpp
    ${source_path}/Draw/DrawVerification.h
    ${source_path}/Draw/DrawState.cpp
    ${source_path}/Draw/PixelFormat.cpp
    ${source_path}/Draw/Program.cpp
    ${source_path}/Draw/ProgramInterface.cpp
    ${source_path}/Draw/ProgramInterfaceFragmentOutput.cpp
    ${source_path}/Draw/ProgramInterfaceSampler.cpp
    ${source_path}/Draw/ProgramInterfaceUniform.cpp
    ${source_path}/Draw/ProgramInterfaceVertexAttribute.cpp
    ${source_path}/Draw/RasterizerState.cpp
    ${source_path}/Draw/StencilState.cpp
    ${source_path}/Draw/StencilState.cpp
    ${source_path}/Draw/Surface.cpp
    ${source_path}/Draw/SurfaceBinary.cpp
    ${source_path}/Draw/Texture.cpp
    ${source_path}/Draw/TextureBinary.cpp
    ${source_path}/Draw/TextureBinding.cpp
    ${source_path}/Draw/TextureLoader.cpp
    ${source_path}/Draw/TextureUploader.cpp
    ${source_path}/Draw/Uniform.cpp
)

# api_includes
set(api_includes
    ${include_path}/Deliberation.h
    ${include_path}/Deliberation_API.h

    ${include_path}/Core/Detail/IBlobImpl.h
    ${include_path}/Core/Assert.h
    ${include_path}/Core/Blob.h
    ${include_path}/Core/Blob.inl
    ${include_path}/Core/FileUtils.h
    ${include_path}/Core/OffsetOf.h
    ${include_path}/Core/OffsetOf.inl
    ${include_path}/Core/Optional.h
    ${include_path}/Core/Optional.inl
    ${include_path}/Core/StringUtils.h
    ${include_path}/Core/Viewport.h

    ${include_path}/Draw/Detail/INamedDataMemberOfImpl.h
    ${include_path}/Draw/Detail/INamedDataMemberOfImpl.inl
    ${include_path}/Draw/Detail/NamedDataMemberOf.h
    ${include_path}/Draw/Detail/NamedDataMemberOf.inl
    ${include_path}/Draw/Detail/UniformImpl.h
    ${include_path}/Draw/GL/GLSLSizeOf.h
    ${include_path}/Draw/GL/GLStateManager.h
    ${include_path}/Draw/GL/GLSLTypeOf.h
    ${include_path}/Draw/GL/GLSLTypeOf.inl
    ${include_path}/Draw/BlendState.h
    ${include_path}/Draw/Buffer.h
    ${include_path}/Draw/BufferLayout.h
    ${include_path}/Draw/BufferLayoutField.h
    ${include_path}/Draw/BufferUpload.h
    ${include_path}/Draw/Clear.h
    ${include_path}/Draw/Context.h
    ${include_path}/Draw/Context.inl
    ${include_path}/Draw/CullState.h
    ${include_path}/Draw/DepthState.h
    ${include_path}/Draw/Draw.h
    ${include_path}/Draw/Draw.inl
    ${include_path}/Draw/DrawState.h
    ${include_path}/Draw/PixelFormat.h
    ${include_path}/Draw/Program.h
    ${include_path}/Draw/ProgramInterface.h
    ${include_path}/Draw/ProgramInterfaceFragmentOutput.h
    ${include_path}/Draw/ProgramInterfaceUniform.h
    ${include_path}/Draw/ProgramInterfaceSampler.h
    ${include_path}/Draw/ProgramInterfaceVertexAttribute.h
    ${include_path}/Draw/RasterizerState.h
    ${include_path}/Draw/StencilState.h
    ${include_path}/Draw/Surface.h
    ${include_path}/Draw/SurfaceBinary.h
    ${include_path}/Draw/Texture.h
    ${include_path}/Draw/TextureBinary.h
    ${include_path}/Draw/TextureBinding.h
    ${include_path}/Draw/TextureLoader.h
    ${include_path}/Draw/TextureUploader.h    
    ${include_path}/Draw/Uniform.h
    ${include_path}/Draw/Uniform.inl
)

# libs
set(libs
    ${GLBINDING_LIBRARY}
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${SDL2_LIBRARY}
)

# library
add_library(Deliberation ${api_includes} ${sources})
target_link_libraries(Deliberation ${libs})
